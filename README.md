# TravellingSalesmanProblem
【Japanese version below】  
The travelling salesman problem, also known as the travelling salesperson problem, asks the following question: "Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?

In construction_method.c, I used the combine of these algorithms:
- Nearest Neighbor
- Nearest Insertion
- Cheapest Insertion
- Furthest Insertion
- Minimum Spanning Tree

In improve_method.c, I prefered to use:
- 2-OPTs
- Or-OPT
- Simulate Annealing

I had an idea of implementing Christofides algorithm using MST and Minimum Weight Perfect Matching to get a result approximately 1.5 compared to the perfect one. But due to the limit of time, I have not written it yet.

# 巡回セールスマン問題
巡回セールスマン問題、または巡回セールスパーソン問題は、次のような問い“都市のリストと各ペアの都市間の距離が与えられたとき、各都市をちょうど一度ずつ訪れ、出発地に戻る最短の可能なルートは何か？”を投げかけます。

construction_method.cでは、これらのアルゴリズムの組み合わせを使用しました：

- 最近傍法
- 最近挿入法
- 最安挿入法
- 最遠挿入法
- 最小全域木
  
improve_method.cでは、以下を好んで使用しました：
- 2-OPT法
- Or-OPT法
- シミュレーテッドアニーリング
