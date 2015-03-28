Eric Tew
Nicolas Chaim
Chris Bentson

Assignment 3 Milestone 2

The multiplayer part is almost done. We are sending the paddle’s position of each player to each other, as well as the computed ball position from the server to the client. When the game is started, the server and client discover each other using broadcasts, and as such, there is no need to enter host names or addresses. 

In a small change to our design plan, we have decided to build a co-op mode instead, where one player controls the left half with their paddle and the other player controls the right half of the paddle. Each player can only move at most to the center and in their respective directions. 

Chris is still figuring out how CEGUI works, which might be implemented for this final submission. Eric and Nick have been working on multiplayer functionality, including game state synchronization. 

For the most part, we are following the code architecture as planned. The networking functionality is implemented in a single class providing simple server/client functionality based on standard BSD sockets. Else, our previous software architecture has been easy to extend without any major changes.

Overall, we feel confident about our progress and are on track for our final deliverable.

Instructions:
1) Go to the project root and type: cmake -G"Unix Makefiles"
2) Type: make -j8
3) Run the game:
   - Computer 1 (client): ./OgrePinball
   - Computer 2 (server): ./OgrePinball 1

