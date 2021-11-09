This report is written by Petro Mudrievskyj to describe the algorithm used in his solution to the problem of the 3rd round of [Huawei Optimization Tournament](https://algotester.com/hot/en).
You can find the statement [here](statement.pdf "Graph Efficiency").

### Report

Let `d[u][v]` be the distance between nodes `u` and `v`. Given the input we can calculate it with Floyd's algorithm.  
If we want to insert single shortcut into the graph then it's pretty easy to find the optimal one - we just have to consider all of them and pick the one that increases `Eff(G)` the most.  
Difference in efficiency created by inserting edge `(u, v)` can be calculated like this
```python
diff = 0
for i in V:
  for j in V:
    d' = min(d[i][u] + d[v][j], d[i][v] + d[u][j]) + 1
    if d' < d[i][j]:
      diff += 1 / d'
      diff -= 1 / d[i][j]
```
We can further optimize it by inserting following check in the first loop: `abs(d[i][v] - d[i][u]) > 1` - it's obvious that if shortcut doesn't decrease distance from node `i` to one of the endpoints, then it doesn't decrease distance from `i` to any of the nodes.

Now we can use this algorithm to greedily find five succesive shortcuts in a given graph.  
But we can do better by considering more options. Exact branching coefficients can me found in the source code, but the main idea is following: take some (say, `n`) best shortcuts on the first step, then for each of them take some (say, `n/2`) best shortcuts on the second step, and so on, until you get enough shortcuts, calculate efficiency, and out of all of those pick the best solution you've found (and don't run out of time!).
So the algorithm is "greedy with branching" and it gave 2'881'077'881 points in final testing.
