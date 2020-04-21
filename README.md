# Computational Geometry

## Assignment 1
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