# Elevator Pitch
Cascade is a multi-scale transportation network simulation engine that models the complex feedback loops between public transit networks and road traffic. Built in C for performance-critical components and C++ for OOP architecture, it sumlates how macro-level transit decisions (routes, schedules, capacity) interact with micro-level traffic dynamics (vehicle and pedestrian movement, congestion, signal timing) to create emergent urban mobility patterns, including cascading failures where poor transit reliability drives more people to cars, creating congestion that makes transit even worse.

Think Mini Metro meets Cities: Skylines, but with realistic feedback beteween the scales.

# Project Scope

## Macro Scale: Transit Network Simulation

### Network Infrastructure
- Graph-based transit network (stations as nodes, routes as edges)
- Passenger spawning at homes and despawning at destinations
- Station capacity and passenger queuing
- Basic network topology

### Transit Vehicles
- Buses/trains following scheduled routes
- Vehicle capacity limits
- Dwell time at stations (loading/unloading)
- Simple breakdown/delay modeling

### Passenger Agents
- Origin-destination pairs with time preferences
- Route planning using pathfinding
- Waiting and boarding behavior


### Transit Management
- Fleet scheduling and dispatch
- Passenger demand tracking
- Service reliability metrics
- Basic headway management

### Seasonal Effects
- Holiday/Weekend effects on demand spikes and dips
- Weather effects on transit

## Micro Scale: Traffic Simulation

### Road and Footpath Network
- Grid-based road network with intersectionss
- Multi-lane roads and streets
- Turn lanes an intersection geometry
- Any-way footpaths

### Vehicle Agents
- Private cars with origin-destination routing
- Car-following model
- Lane selection and lane changin
- Acceleration/braking physics

### Traffic Control
- Fixed-cycle traffic signals
- Signal coordination
- Turn restrictions
- Queue management at intersections
- Adaptive traffic signals

### Traffic Management
- Vehicle spawning based on demand
- Congestion detection and measurement
- Travel time tracking
- Throughput monitoring

### Non-Vehicles
- Pedestrians and cyclist simulations
- Individual abandonment thresholds for excessive wait times and seeking of alternative transport systems (e.g. Uber)
- Individual economics and fare tolerances to simulate Car-Public Transport choices
- Individual multi-modal transfers (e.g. drive to train station, take the train and then walk to work from your destination)

### Accidents
- Traffic accident simulations
- Obstacle detection to avoid traffic accidents


## Integration Layer: Cross-Scale Interactions
### Transit - Traffic Coupling
- Transit vehicles as agents in traffic simulation
- Road congestion affects bus arrival times
- Bus delays propagate through schedule

### Modal Choice
- Passengers choose between transit and driving based on:
    - Expected travel time
    - Transit reliability (historical performance)
    - Simple utility function

### Feedback Mechanisms
- Transit delays → passenger abandonment → more cars
- More cars → worse congestion → slower buses
- Death spiral detection and metrics

### Synchronization
- Event-based coupling between scales
- Shared time stepping
- Cross-scale message passing

# MVP
- 2D grid-based network
- Basic vehicle movement and - passenger flow
- Simple transit vs. car mode choice
- Visualizer

# Tech Stack
- C for performance critical components
- C++ for OOP