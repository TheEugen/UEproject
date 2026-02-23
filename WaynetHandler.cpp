<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.


#include "WaynetHandler.h"

#include <limits>

WaynetHandler::WaynetHandler(TArray<AActor*>& actor_waynet)
{
    for(AActor* actor : actor_waynet)
        waynet.Add(Cast<AWaypoint>(actor));
}

TArray<AWaypoint*>& WaynetHandler::getWaynet()
{
    return waynet;
}

TArray<AWaypoint*> WaynetHandler::findPath(AWaypoint* start, AWaypoint* end)
{
    check(start);
    check(end);
    check(start != end);

    /*
    TQueue<AWaypoint*> fringe; // nodes to look at || "Die Realisierung der Funktion „INSERT“ bestimmt, welches Suchverfahren hier
                              // verfolgt wird (Tiefensuche, Breitensuche, Best First Seach, UCS, A*)."
    */

	TArray<AWaypoint*> pathToEnd;
	AWaypoint* node = nullptr;
	bool bPathFound = false;
    TSet<AWaypoint*> closedList;
    TQueue<AWaypoint*> fringe;
    TMap<AWaypoint*, AWaypoint*> vorgaenger;

    // initialize vorgänger map
    for(int i = 0; i < waynet.Num(); ++i)
        vorgaenger.Add(waynet[i], nullptr);

    // add the start node to nodes to look at
    fringe.Enqueue(start);
    while( !fringe.IsEmpty() )
    {
        // get node to look at
        fringe.Dequeue(node); // INSERT

        // if we have the target we are done
        if( node == end )
        {
            bPathFound = true;
            break;
        }

        // if we haven't looked at the node yet, add it to looked-at-list
        if( !closedList.Contains(node) )
            closedList.Add(node);

        // check if wp is dead end, passthrough or crossing
        switch( node->getNextNodes().Num() )
        {
            // wp is dead end
        case 1:
            if( !closedList.Contains(node->getNextNodes()[0]) )
            {
                vorgaenger[node->getNextNodes()[0]] = node;
                fringe.Enqueue(node->getNextNodes()[0]);
                closedList.Add(node->getNextNodes()[0]);
            }
            break;
            // wp is normal passthrough
        case 2:
            for(int i = 0; i < 2; ++i)
            {
                if( node != node->getNextNodes()[i])
                {
                    if( !closedList.Contains(node->getNextNodes()[i]) )
                    {
                        vorgaenger[node->getNextNodes()[i]] = node;
                        fringe.Enqueue(node->getNextNodes()[i]);
                        closedList.Add(node->getNextNodes()[i]);
                    }
                }
            }
            break;
            // wp is crossing
        default:
            for(int i = 0; i < node->getNextNodes().Num(); ++i)
            {
                if( !closedList.Contains(node->getNextNodes()[i]))
                {
                    vorgaenger[node->getNextNodes()[i]] = node;
                    fringe.Enqueue(node->getNextNodes()[i]);
                    closedList.Add(node->getNextNodes()[i]);
                }
            }
            break;
        }
    }

    // if there's no path then there are missing connections between wps
    if( !bPathFound )
        UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: no path found"));

    // build path backwards
    while( node )
    {
        pathToEnd.Insert(node, 0);
        node = vorgaenger[node];
    }

    return pathToEnd;
}

WaynetHandler::WaynetHandler()
{
}

WaynetHandler::~WaynetHandler()
{
}

TArray<AWaypoint*> WaynetHandler::testGraphSearch(AWaypoint* start, AWaypoint* end)
{
    check(start);
    check(end);
    check(start != end);

    // the final path to return
    TArray<AWaypoint*> pathToEnd;

    // current node which we look at
    AWaypoint* node = nullptr;

    // flag for error checking
    bool bPathFound = false;

    // nodes we already looked at
    TSet<AWaypoint*> closedList;

    // nodes we'll look at while BFS
    TQueue<AWaypoint*> fringe_bfs;

    // saving the current height and the distances of the nodes of each height,
    // to determine which node to look at next when we reach a dead end
    TArray<TMap<AWaypoint*, float>> height_distances; //obj[height]->[node][distance]
    uint32 current_height = 0;

    // while dfs, we'll also calculate the distance between the child we otherwise 
    // would add to the fringe and save it in h_current, if we encounter a child with a lower value,
    // this child will be set to next_node_dfs
    AWaypoint* next_node_dfs = nullptr;
   
    // distance between node to look at and goal
    float h_current = std::numeric_limits<float>::max();

    // flag to determine which algorithm we are currently using
    bool dfs_active = true;

    // dead end flag
    bool dead_end = false;

    // saving the previous node so we can build the final path backwards later
    TMap<AWaypoint*, AWaypoint*> vorgaenger;

    // distance between start and goal
    float h = FVector::Distance(start->GetActorLocation(), end->GetActorLocation());

    // h_switch -> value when to switch from DFS to BFS
    float h_switch = 0.25f;

    // initialize vorgänger map
    for(int i = 0; i < waynet.Num(); ++i)
        vorgaenger.Add(waynet[i], nullptr);

    // end node location
    FVector end_loc = end->GetActorLocation();

    // set first node to look at to start
    next_node_dfs = start;

    // TODO while handling distances, only compare when z axis is even
    // TODO implement fringe_dfs
    // what to do when dead end is encountered while bfs

    while( !fringe_bfs.IsEmpty() || dfs_active )
    {
        // DFS
        if( dfs_active )
        {
            // in the previous iteration, we encountered a dead end
            if( dead_end )
            {
                // next_node_dfs = height_distances[current_height - i] -> child with lowest value
                bool found = false;
                float distance_tmp = std::numeric_limits<float>::max();

                while( !found )
                {
                    for(int i = current_height - 1; i >= 0; --i)
                    {
                        // if we encountered a dead end and go backwards, we
                        // need to find the first node which isnt a passthrough
                        if( height_distances[i].Num() > 1 )
                        {
                            // if we found a crossing, choose the children with 
                            // minimum distance to target
                            for(const TPair<AWaypoint*, float>& map : height_distances[i])
                            {
                                if( map.Value < distance_tmp )
                                {
                                    next_node_dfs = map.Key;
                                    distance_tmp = map.Value;
                                    found = true;
                                }
                            }  
                        }

                        // prevent accessing invalid index
                        if( i == 0 && !found )
                            UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: testGraphSearch failed to find child to continue with"));
                    }
                }

                // reset the flag
                dead_end = false;
            }

            // get the node to look at
            node = next_node_dfs;

            if( !closedList.Contains(node) )
                closedList.Add(node);

            // check if we should switch to bfs
            if( FVector::Distance(node->GetActorLocation(), end->GetActorLocation()) <= h * h_switch )
            {
                dfs_active = false;
                fringe_bfs.Enqueue(node);
                next_node_dfs = nullptr;
                continue;
            }

            // if we look at the target we are done
            if( node == end )
            {
                bPathFound = true;
                break;
            }

            TMap<AWaypoint*, float> distances_tmp;

            // check if wp is dead end, passthrough or crossing
            switch( node->getNextNodes().Num() )
            {
            case 1: // wp is dead end
            {
                if( node != node->getNextNodes()[0] )
                {
                    if( !closedList.Contains(node->getNextNodes()[0]) )
                    {
                        vorgaenger[node->getNextNodes()[0]] = node;               
                        FVector child_loct = node->getNextNodes()[0]->GetActorLocation();                    
                        float child_distancet = FVector::Distance(child_loct, end_loc);
                        distances_tmp.Add(node->getNextNodes()[0], child_distancet);
                        
                        next_node_dfs = node->getNextNodes()[0];
                            
                        closedList.Add(node->getNextNodes()[0]);
                    }
                }
                else
                    dead_end = true;
                break;
            }    
            // wp is normal passthrough, no need to calculate distance
            case 2:
            {
                for(int i = 0; i < 2; ++i)
                {
                    if( node != node->getNextNodes()[i] )
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                        
                            FVector child_loc = node->getNextNodes()[i]->GetActorLocation();                    
                            float child_distance = FVector::Distance(child_loc, end_loc);
                            distances_tmp.Add(node->getNextNodes()[i], child_distance);

                            next_node_dfs = node->getNextNodes()[i];

                            closedList.Add(node->getNextNodes()[i]);
                        }   
                    }
                }
                break;
            }
            // wp is crossing
            default:
            {
                for(int i = 0; i < node->getNextNodes().Num(); ++i)
                {
                    if( node != node->getNextNodes()[i] )
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                        
                            FVector child_loc = node->getNextNodes()[i]->GetActorLocation();
                            float child_distance = FVector::Distance(child_loc, end_loc);
                            distances_tmp.Add(node->getNextNodes()[i], child_distance);

                            if( child_distance < h_current)
                            {
                                next_node_dfs = node->getNextNodes()[i];
                                h_current = child_distance;
                            }

                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
            }
            }

            ++current_height;
            height_distances.Add(distances_tmp);

        }
        // BFS
        else
        {
            // encountered dead end and switched
            if( dead_end )
            {
                dfs_active = true;
                continue;
            }

            // get node to look at
            fringe_bfs.Dequeue(node); // INSERT

            // if we look at the target we are done
            if( node == end )
            {
                bPathFound = true;
                break;
            }

            // if we haven't looked at the node yet, add it to looked-at-list
            if( !closedList.Contains(node) )
                closedList.Add(node);

            // check if wp is dead end, passthrough or crossing
            switch( node->getNextNodes().Num() )
            {
                // wp is dead end
            case 1:
            {
                if( node != node->getNextNodes()[0] )
                {
                    if( !closedList.Contains(node->getNextNodes()[0]) )
                    {
                        vorgaenger[node->getNextNodes()[0]] = node;
                        fringe_bfs.Enqueue(node->getNextNodes()[0]);
                        closedList.Add(node->getNextNodes()[0]);
                    }
                }
                break;
            }
            // wp is normal passthrough
            case 2:
                for(int i = 0; i < 2; ++i)
                {
                    if( node != node->getNextNodes()[i])
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                            fringe_bfs.Enqueue(node->getNextNodes()[i]);
                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
                // wp is crossing
            default:
                for(int i = 0; i < node->getNextNodes().Num(); ++i)
                {
                    if( node != node->getNextNodes()[i])
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]))
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                            fringe_bfs.Enqueue(node->getNextNodes()[i]);
                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
            }
        }
    }

    // if there's no path then there are missing connections between wps
    if( !bPathFound )
        UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: no path found"));

    // build path backwards
    while( node )
    {
        pathToEnd.Insert(node, 0);
        node = vorgaenger[node];
    }

    return pathToEnd;
=======
// Fill out your copyright notice in the Description page of Project Settings.


#include "WaynetHandler.h"

#include <limits>

WaynetHandler::WaynetHandler(TArray<AActor*>& actor_waynet)
{
    for(AActor* actor : actor_waynet)
        waynet.Add(Cast<AWaypoint>(actor));
}

TArray<AWaypoint*>& WaynetHandler::getWaynet()
{
    return waynet;
}

TArray<AWaypoint*> WaynetHandler::findPath(AWaypoint* start, AWaypoint* end)
{
    check(start);
    check(end);
    check(start != end);

    /*
    TQueue<AWaypoint*> fringe; // nodes to look at || "Die Realisierung der Funktion „INSERT“ bestimmt, welches Suchverfahren hier
                              // verfolgt wird (Tiefensuche, Breitensuche, Best First Seach, UCS, A*)."
    */

	TArray<AWaypoint*> pathToEnd;
	AWaypoint* node = nullptr;
	bool bPathFound = false;
    TSet<AWaypoint*> closedList;
    TQueue<AWaypoint*> fringe;
    TMap<AWaypoint*, AWaypoint*> vorgaenger;

    // initialize vorgänger map
    for(int i = 0; i < waynet.Num(); ++i)
        vorgaenger.Add(waynet[i], nullptr);

    // add the start node to nodes to look at
    fringe.Enqueue(start);
    while( !fringe.IsEmpty() )
    {
        // get node to look at
        fringe.Dequeue(node); // INSERT

        // if we have the target we are done
        if( node == end )
        {
            bPathFound = true;
            break;
        }

        // if we haven't looked at the node yet, add it to looked-at-list
        if( !closedList.Contains(node) )
            closedList.Add(node);

        // check if wp is dead end, passthrough or crossing
        switch( node->getNextNodes().Num() )
        {
            // wp is dead end
        case 1:
            if( !closedList.Contains(node->getNextNodes()[0]) )
            {
                vorgaenger[node->getNextNodes()[0]] = node;
                fringe.Enqueue(node->getNextNodes()[0]);
                closedList.Add(node->getNextNodes()[0]);
            }
            break;
            // wp is normal passthrough
        case 2:
            for(int i = 0; i < 2; ++i)
            {
                if( node != node->getNextNodes()[i])
                {
                    if( !closedList.Contains(node->getNextNodes()[i]) )
                    {
                        vorgaenger[node->getNextNodes()[i]] = node;
                        fringe.Enqueue(node->getNextNodes()[i]);
                        closedList.Add(node->getNextNodes()[i]);
                    }
                }
            }
            break;
            // wp is crossing
        default:
            for(int i = 0; i < node->getNextNodes().Num(); ++i)
            {
                if( !closedList.Contains(node->getNextNodes()[i]))
                {
                    vorgaenger[node->getNextNodes()[i]] = node;
                    fringe.Enqueue(node->getNextNodes()[i]);
                    closedList.Add(node->getNextNodes()[i]);
                }
            }
            break;
        }
    }

    // if there's no path then there are missing connections between wps
    if( !bPathFound )
        UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: no path found"));

    // build path backwards
    while( node )
    {
        pathToEnd.Insert(node, 0);
        node = vorgaenger[node];
    }

    return pathToEnd;
}

WaynetHandler::WaynetHandler()
{
}

WaynetHandler::~WaynetHandler()
{
}

TArray<AWaypoint*> WaynetHandler::testGraphSearch(AWaypoint* start, AWaypoint* end)
{
    check(start);
    check(end);
    check(start != end);

    // the final path to return
    TArray<AWaypoint*> pathToEnd;

    // current node which we look at
    AWaypoint* node = nullptr;

    // flag for error checking
    bool bPathFound = false;

    // nodes we already looked at
    TSet<AWaypoint*> closedList;

    // nodes we'll look at while BFS
    TQueue<AWaypoint*> fringe_bfs;

    // saving the current height and the distances of the nodes of each height,
    // to determine which node to look at next when we reach a dead end
    TArray<TMap<AWaypoint*, float>> height_distances; //obj[height]->[node][distance]
    uint32 current_height = 0;

    // while dfs, we'll also calculate the distance between the child we otherwise 
    // would add to the fringe and save it in h_current, if we encounter a child with a lower value,
    // this child will be set to next_node_dfs
    AWaypoint* next_node_dfs = nullptr;
   
    // distance between node to look at and goal
    float h_current = std::numeric_limits<float>::max();

    // flag to determine which algorithm we are currently using
    bool dfs_active = true;

    // dead end flag
    bool dead_end = false;

    // saving the previous node so we can build the final path backwards later
    TMap<AWaypoint*, AWaypoint*> vorgaenger;

    // distance between start and goal
    float h = FVector::Distance(start->GetActorLocation(), end->GetActorLocation());

    // h_switch -> value when to switch from DFS to BFS
    float h_switch = 0.25f;

    // initialize vorgänger map
    for(int i = 0; i < waynet.Num(); ++i)
        vorgaenger.Add(waynet[i], nullptr);

    // end node location
    FVector end_loc = end->GetActorLocation();

    // set first node to look at to start
    next_node_dfs = start;

    // TODO while handling distances, only compare when z axis is even
    // TODO implement fringe_dfs
    // what to do when dead end is encountered while bfs

    while( !fringe_bfs.IsEmpty() || dfs_active )
    {
        // DFS
        if( dfs_active )
        {
            // in the previous iteration, we encountered a dead end
            if( dead_end )
            {
                // next_node_dfs = height_distances[current_height - i] -> child with lowest value
                bool found = false;
                float distance_tmp = std::numeric_limits<float>::max();

                while( !found )
                {
                    for(int i = current_height - 1; i >= 0; --i)
                    {
                        // if we encountered a dead end and go backwards, we
                        // need to find the first node which isnt a passthrough
                        if( height_distances[i].Num() > 1 )
                        {
                            // if we found a crossing, choose the children with 
                            // minimum distance to target
                            for(const TPair<AWaypoint*, float>& map : height_distances[i])
                            {
                                if( map.Value < distance_tmp )
                                {
                                    next_node_dfs = map.Key;
                                    distance_tmp = map.Value;
                                    found = true;
                                }
                            }  
                        }

                        // prevent accessing invalid index
                        if( i == 0 && !found )
                            UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: testGraphSearch failed to find child to continue with"));
                    }
                }

                // reset the flag
                dead_end = false;
            }

            // get the node to look at
            node = next_node_dfs;

            if( !closedList.Contains(node) )
                closedList.Add(node);

            // check if we should switch to bfs
            if( FVector::Distance(node->GetActorLocation(), end->GetActorLocation()) <= h * h_switch )
            {
                dfs_active = false;
                fringe_bfs.Enqueue(node);
                next_node_dfs = nullptr;
                continue;
            }

            // if we look at the target we are done
            if( node == end )
            {
                bPathFound = true;
                break;
            }

            TMap<AWaypoint*, float> distances_tmp;

            // check if wp is dead end, passthrough or crossing
            switch( node->getNextNodes().Num() )
            {
            case 1: // wp is dead end
            {
                if( node != node->getNextNodes()[0] )
                {
                    if( !closedList.Contains(node->getNextNodes()[0]) )
                    {
                        vorgaenger[node->getNextNodes()[0]] = node;               
                        FVector child_loct = node->getNextNodes()[0]->GetActorLocation();                    
                        float child_distancet = FVector::Distance(child_loct, end_loc);
                        distances_tmp.Add(node->getNextNodes()[0], child_distancet);
                        
                        next_node_dfs = node->getNextNodes()[0];
                            
                        closedList.Add(node->getNextNodes()[0]);
                    }
                }
                else
                    dead_end = true;
                break;
            }    
            // wp is normal passthrough, no need to calculate distance
            case 2:
            {
                for(int i = 0; i < 2; ++i)
                {
                    if( node != node->getNextNodes()[i] )
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                        
                            FVector child_loc = node->getNextNodes()[i]->GetActorLocation();                    
                            float child_distance = FVector::Distance(child_loc, end_loc);
                            distances_tmp.Add(node->getNextNodes()[i], child_distance);

                            next_node_dfs = node->getNextNodes()[i];

                            closedList.Add(node->getNextNodes()[i]);
                        }   
                    }
                }
                break;
            }
            // wp is crossing
            default:
            {
                for(int i = 0; i < node->getNextNodes().Num(); ++i)
                {
                    if( node != node->getNextNodes()[i] )
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                        
                            FVector child_loc = node->getNextNodes()[i]->GetActorLocation();
                            float child_distance = FVector::Distance(child_loc, end_loc);
                            distances_tmp.Add(node->getNextNodes()[i], child_distance);

                            if( child_distance < h_current)
                            {
                                next_node_dfs = node->getNextNodes()[i];
                                h_current = child_distance;
                            }

                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
            }
            }

            ++current_height;
            height_distances.Add(distances_tmp);

        }
        // BFS
        else
        {
            // encountered dead end and switched
            if( dead_end )
            {
                dfs_active = true;
                continue;
            }

            // get node to look at
            fringe_bfs.Dequeue(node); // INSERT

            // if we look at the target we are done
            if( node == end )
            {
                bPathFound = true;
                break;
            }

            // if we haven't looked at the node yet, add it to looked-at-list
            if( !closedList.Contains(node) )
                closedList.Add(node);

            // check if wp is dead end, passthrough or crossing
            switch( node->getNextNodes().Num() )
            {
                // wp is dead end
            case 1:
            {
                if( node != node->getNextNodes()[0] )
                {
                    if( !closedList.Contains(node->getNextNodes()[0]) )
                    {
                        vorgaenger[node->getNextNodes()[0]] = node;
                        fringe_bfs.Enqueue(node->getNextNodes()[0]);
                        closedList.Add(node->getNextNodes()[0]);
                    }
                }
                break;
            }
            // wp is normal passthrough
            case 2:
                for(int i = 0; i < 2; ++i)
                {
                    if( node != node->getNextNodes()[i])
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]) )
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                            fringe_bfs.Enqueue(node->getNextNodes()[i]);
                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
                // wp is crossing
            default:
                for(int i = 0; i < node->getNextNodes().Num(); ++i)
                {
                    if( node != node->getNextNodes()[i])
                    {
                        if( !closedList.Contains(node->getNextNodes()[i]))
                        {
                            vorgaenger[node->getNextNodes()[i]] = node;
                            fringe_bfs.Enqueue(node->getNextNodes()[i]);
                            closedList.Add(node->getNextNodes()[i]);
                        }
                    }
                }
                break;
            }
        }
    }

    // if there's no path then there are missing connections between wps
    if( !bPathFound )
        UE_LOG(LogTemp, Fatal, TEXT("WaynetHandler: no path found"));

    // build path backwards
    while( node )
    {
        pathToEnd.Insert(node, 0);
        node = vorgaenger[node];
    }

    return pathToEnd;
>>>>>>> 8a07602be94d9eb358cc1211dc742f7addd23f26
}