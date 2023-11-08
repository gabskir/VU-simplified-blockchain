# VU-simplified-blockchain
## Simplified Blockchain Simulation
A simpled simulation of a blockchain mechanism, developed according to the requirements of the first task of the "Blockchain Technology" module at Vilnius University. <br>
It's an educational resource meant to demonstrate the basic concepts of blockchain operations, including creating transactions, blocks, mining, and managing a ledger of transactions with proof-of-work.

### Requirements:
---

1. User Creation
  - Generate approximately 1000 network users with the following attributes:
    - Name
    - Public hash key (`public_key`)
    - Random currency balance, ranging from 100 to 1,000,000 units.
2. Transaction Pool
  - Generate a pool of approximately 10,000 new transactions that are not yet included in any block. Transaction structure should include at least the following attributes:
    - Transaction ID (hash of the other transaction fields)
    - Sender (public key of the sender)
    - Recipient (public key of the recipient)
    - Amount
3. Block Structure
  - Randomly select about 100 transactions from the transaction pool to insert into a new block. Assume that a new block can contain about 100 transactions.
  - The necessary block structure and required attributes are illustrated below:
  ![block structure figure](https://github.com/gabskir/VU-simplified-blockchain/blob/main/Block_structure.png?raw=true).
4. Mining Process
  - Implement a mining process to find a hash for the new block that meets the **Difficulty Target** requirement. The number of leading zeros in the hash should be determined based on the properties and efficiency of the hash function used.
  - Use the hash function created for a previous assignment for the hashing process.
5. Post-Mining Actions
  - Upon finding a hash that meets the required properties for the new block:
    - Remove the transactions assigned to the new block from the transaction pool.
    - Execute the transactions, i.e., update the balances of the network users.
    - Add the new block to the blockchain chain.
6. Merkle Tree Hash
  - For simplification, instead of a complex **binary Merkle tree** hash implementation, you can use the hash of all transaction IDs that are put into the new block.
7. Mining Logic Improvement
  - Instead of creating one, create five new potential blocks (candidates) from the transaction pool, with about 100 randomly chosen transactions that may overlap in these block candidates.
  - Randomly select one of these five block candidates and perform the mining process for a fixed period (e.g., up to 5 seconds) or a fixed maximum number of hash attempts (e.g., up to 100,000).
  - If the required hash is not found for the randomly selected block candidate after this time, repeat the process with another randomly selected remaining candidate.
  - If no hash with the required properties is found after retrying with all five block candidates, then extend the mining process (e.g., up to 10 seconds) or increase the maximum number of hash attempts (e.g., up to 200,000) and repeat the process.

### Features:
---

- Transaction creation and management
- Block creation with a Merkle tree for transaction integrity
- Proof-of-work mining simulation
- Blockchain integrity checks
- Balance management for simulated users

### Project Structure:
---

The project consists of several components, each handling a different aspect of the blockchain:

- `mylib.h`: Common includes and utility functions required across the project.
- `randomizer.h`: Handles the generation of random values, crucial for various simulation aspects, including user creation and transaction processing.
- `user.h`: Defines the `User` and `UserManager` classes for representing users within the blockchain network, managing their attributes such as name, public key, and balance.
- `transactions.h`: Outlines the `Transaction` and `TransactionBuilder` classes, which are responsible for creating and managing transactions within the network.
- `merkleTree.h`: Implements a Merkle tree that provides a secure and efficient structure for summarizing all the transactions in a block.
- `block.h`: Contains the `Block` class for creating new blocks and the `Mine` class for simulating the mining process.
- `functions.h`: Additional utility functions that may be used for hashing and other operations.
- `hashing.cpp`: Contains the implementation of the hashing function used for creating transaction IDs and block hashes.
- `main.cpp`: The entry point of the simulation, orchestrating the entire blockchain process from user and transaction generation to block mining and chaining.

### Main Simulation Process
---

The `main.cpp` serves as the entry point of the blockchain simulation and executes the following key steps:

1. **User Initialization**: 
   - Creates a `UserManager` with approximately 1000 users, each with a name, public key, and random currency balance.
   
2. **Transaction Pool Generation**: 
   - Utilizes a `TransactionBuilder` to generate around 10,000 new transactions, forming a pool of pending transactions that have not been added to any block yet.

3. **Genesis Block Creation**: 
   - Constructs a genesis block, which is the first block in the blockchain, with a predefined difficulty target.

4. **Blockchain Initialization**: 
   - Initializes a blockchain structure using a stack to store the blocks, pushing the genesis block as the first element.

5. **Miner Setup**: 
   - Initializes a set of miners (5 in this case), each capable of attempting to mine a block.

6. **Mining Loop**: 
   - Enters a loop that continues as long as there are transactions left in the pool.
   - Inside the loop:
     - Randomly selects a subset of transactions (100 by default) to form a candidate block.
     - Each miner then tries to mine this block, one at a time, until a valid hash is found that satisfies the difficulty target, or until all miners have exhausted their attempts.
     - If a miner successfully mines a block, the transactions are verified, user balances are updated, and the block is added to the blockchain.
     - If no miner succeeds, the number of mining attempts is doubled for the next round.
   
7. **Completion**: 
   - Once all transactions have been processed, or there are no transactions left, the simulation ends.
   - Outputs the total number of blocks in the blockchain and the number of blocks each miner has successfully mined.

The simulation provides visibility into the mining process, showing real-time output during block and transaction creation, and when miners successfully mine a block. This output is designed to be informative and visually similar to real-world blockchain explorers.

#### Real-time output examples:
##### Information about created transaction
  
   ###### Valid transaction:<br>
   
   ![transaction_Info](https://github.com/gabskir/VU-simplified-blockchain/blob/main/information_about_valid_transactions.png?raw=true).
      
   ###### Invalid transaction:<br>
   ![invalid_transaction_Info](https://github.com/gabskir/VU-simplified-blockchain/blob/main/invalid_transaction.png?raw=true)
##### Block creation output 
   ![block_Info](https://github.com/gabskir/VU-simplified-blockchain/blob/main/block_Information.png?raw=true) <br>

### Usage
---
To run the simulation, download all .cpp and .h files, compile the project using a C++ compiler and execute the resulting binary. Example

```
g++ -c main.cpp hashing.cpp 
g++ main.o hashing.o -o hashing
./hashing

```



