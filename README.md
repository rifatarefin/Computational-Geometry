# Computational Geometry Topics
All algorithms are implemented in C++. OpenGL is used for graphical representation.
## Computation of Convex Hull
The problem description is available [here](<Offline 1/Assignment 1.pdf>)

### Time Complexity of Graham Scan Algorithm:
Time complexity of Graham Scan algorithm is always *`O(nlogn)`* irrespective of the input. *`O(nlogn)`* time is needed while sorting the vertices according to their angle with the most bottom-right vertex.

|1. *Graham Scan* performs better than *Gift Wrapping*| 2. *Graham Scan* performs worse than *Gift Wrapping* |
| :------------ | :------------ |
| ![](<Offline 1/Testcases/1.png>) | ![](<Offline 1/Testcases/2.png>)  |
| **Complexity** <br> Graham Scan: *`O(nlogn)`*<br> Gift Wrapping: *`O(n^2)`* |  **Complexity** <br> Graham Scan: *`O(nlogn)`*<br> Gift Wrapping: *`O(nh)`*; here *`h=4`* |

|3. *Graham Scan* performs better than *Quick Hull*| 4. *Graham Scan* performs same as *Quick Hull* |
| :------------ | :------------ |
| ![](<Offline 1/Testcases/3.png>) | ![](<Offline 1/Testcases/4.png>)  |
| **Complexity** <br> Graham Scan: *`O(nlogn)`*<br> Quick Hull: *`O(n^2)`* |  **Complexity** <br> Graham Scan: *`O(nlogn)`*<br> Quick Hull: *`O(nlogn)`* |

## Polygon Partitioning
The problem description is available [here](<Offline 2/Assignment 2.pdf>)

**Data Structure Used:** To store the vertices, I used structure with the following attributes:

- X coordinate, Y coordinate
- Sequence of input
- Vertex type
- Index of the helper vertex

To store the edges, only the starting point is used. As the input is scanned in counter clockwise direction, so the other vertex of the edge must be the next point in input.

A set is maintained to store the edges in T. The `<` Operator was overloaded to maintain the ordering in the set. To find an edge immediately left of a vertex, the index in which this vertex would be inserted was calculated. The edge of the previous index is the immediate left edge.

| ![](<Offline 2/Testcases/1.png>) <br> Input polygon | ![](<Offline 2/Testcases/2.png>) <br> Y-monotone pieces |
| :------------ | :------------ |
| ![](<Offline 2/Testcases/3.png>)<br> **Polygon triangulation** |  ![](<Offline 2/Testcases/4.png>)<br> **Another example** |


## Delaunay Triangulation & Voronoi Diagram
The problem description is available [here](<Offline 3/Assignment 3.pdf>)

| ![](<Offline 3/Testcases/1.png>) <br> Input points | ![](<Offline 3/Testcases/2.png>) <br> Delaunay Triangulation |
| :------------ | :------------ |
| ![](<Offline 3/Testcases/3.png>)<br> **Voronoi Diagram** |  ![](<Offline 3/Testcases/4.png>)<br> **Another example** |

