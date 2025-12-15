Cascade/
├── .github/
│   └── workflows/
│       └── ci.yml
│
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── settings.json
│   └── tasks.json
│
├── build/
│                          
├── docs/                                   
│   └── STRUCTURE.md
│
├── include/
│   └── cascade/
│       ├── core/
│       │   ├── simulation_observer.hpp
│       │   ├── simulation_stats.hpp
│       │   ├── simulation.hpp
│       │   └── time.hpp
│       ├── environment/
│       │   └── environment.hpp
│       ├── fire/
│       │   ├── fire_model.hpp
│       │   └── simple_fire_model.hpp
│       ├── grid/
│       │   ├── cell_state.hpp
│       │   ├── cell.hpp
│       │   └── grid.hpp
│       ├── gui/
│       │   ├── game_ui.hpp
│       │   └── sdl_renderer.hpp
│       ├── supression/
│       │   ├── agent_factory.hpp
│       │   ├── aggressive_agent.hpp
│       │   ├── defensive_agent.hpp
│       │   └── supression_agent.hpp
│       ├── ui/
│       │   └── input_helper.hpp
│       └── utils/
│           ├── agent_config.hpp
│           ├── config_builder.hpp
│           ├── config.hpp
│           ├── validation.hpp
│           └── vector2d.hpp
│
├── src/
│   ├── core/
│   │   ├── simulation.cpp
│   │   └── time.cpp
│   ├── environment/
│   ├── fire/
│   │   ├── fire_model.cpp
│   │   └── simple_fire_model.cpp
│   ├── grid/
│   │   ├── cell.cpp
│   │   └── grid.cpp
│   ├── gui/
│   │   ├── game_ui.cpp
│   │   └── sdl_renderer.cpp
│   ├── supression/
│   │   ├── agent_factory.cpp
│   │   ├── aggressive_agent.cpp
│   │   ├── defensive_agent.cpp
│   │   └── supression_agent.cpp
│   ├── utils/
│   │   ├── agent_config.cpp
│   │   ├── config_builder.cpp
│   │   ├── config.cpp
│   │   └── input_helper.cpp
│   ├── CMakeLists.cpp
│   └── main.cpp
│
├── tests/
│   ├── integration/
│   │   ├── test_fire_spread_scenario.cpp
│   ├── unit/
│   │   ├── grid/
│   │   │   ├── test_cell.cpp
│   │   │   └── test_grid.cpp
│   │   ├── utils/
│   │   │   ├── test_config.cpp
│   │   │   ├── test_time.cpp
│   │   │   └── test_vector2d.cpp
│   │   ├── test_simulation.cpp
│   │   └── test_simple_fire_model.cpp
│   └── CMakeLists.txt
│
├── .clang-tidy
├── .editorconfig
├── .gitignore                              # Files to exclude from version control
├── CMakeLists.txt
├── README.md                               # Project overview and setup guide
└── vcpkg.json