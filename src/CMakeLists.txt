# 添加可执行文件
add_executable(grami
    main.cpp
    graph.cpp
    pattern.cpp
    miner.cpp
)

# 添加源文件
target_sources(grami PRIVATE
    graph.cpp
    pattern.cpp
    miner.cpp
) 