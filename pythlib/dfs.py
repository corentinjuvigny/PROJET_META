from grille import *

def dfs(visited, graph, node):
    if node not in visited:
        visited.append(node)
        for neighbour in graph[node].aux:
            dfs(visited, graph, neighbour)


def run_dfs(graph, visited):
    visited_len=len(visited)
    dfs(visited, graph, 0)
    return visited[visited_len:]