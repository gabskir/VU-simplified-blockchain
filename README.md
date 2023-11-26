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
  
### Usage
---
To run the simulation, download all .cpp and .h files, compile the project using a C++ compiler and execute the resulting binary. Example

```
g++ -c main.cpp hashing.cpp 
g++ main.o hashing.o -o hashing
./hashing

```

## Changes in V0.3

### Project Structure:
---
The project consists of several components, each handling a different aspect of the blockchain:

- `mylib.h`: Common includes and utility functions required across the project.
- `merkleTree.h`: Implements a Merkle tree that provides a secure and efficient structure for summarizing all the transactions in a block.
- `functions.h`: Additional utility functions that may be used for hashing and other operations.
- `hashing.cpp`: Contains the implementation of the hashing function used for creating transaction IDs and block hashes.
- `randomizer.h`: Defines the DataGenerator class for generating random values, used to simulate users and transactions in a blockchain system.
- `block.h`: Describes the Block class, a key element in blockchain, encapsulating transactions, hashing, and mining functionalities.
- `user.h`: Introduces the User class and UTXO structure for representing blockchain participants and their transaction outputs.
- `transactions.h`: Contains the Transaction class, detailing the structure and information of transactions in the blockchain.
- `main.cpp`: Serves as the primary driver of the blockchain simulation, handling user and transaction generation, block mining, and providing an interactive interface for searching and displaying blockchain data.

### Main Simulation Process
---

The `main.cpp` file orchestrates the core activities of the blockchain simulation, executing these critical phases:

1. **Initialization of Users**:
Generates about 1000 users through DataGenerator, each with a unique public key and an initial set of UTXOs (Unspent Transaction Outputs), representing their digital wallet balance.

2. **Transaction Memory Pool Creation**:
Constructs a memory pool of transactions using DataGenerator, simulating around 10,000 transactions that are yet to be processed or included in any block.

3. **Blockchain Foundation Setup**:
Begins with the creation of an initial 'genesis' block, establishing the start of the blockchain. This block is set with a basic difficulty target for mining.

4. **Blockchain Data Structure Initialization**:
Prepares a blockchain data structure, represented as a vector of blocks, and inserts the genesis block as the first element.

5. **Mining Process Initiation**:
Sets up a mining environment using ***OpenMP*** to parallelize the mining process, simulating multiple miners working concurrently.

6. **Block Mining and Blockchain Expansion**:
Enters a continuous loop, operational as long as there are unprocessed transactions in the memory pool.<br>
***Within the loop***:
- Selects a batch of pending transactions (up to 100) to create a new block.
- Initiates the mining process for this block, employing parallel computation to find a valid hash that meets the difficulty criteria.
- Upon successful mining, verifies transactions, updates balances, and appends the newly mined block to the blockchain.
- If mining is unsuccessful, the process repeats with the next set of transactions.
  
7. **Simulation Conclusion:**
Concludes once all transactions are processed or when the transaction pool is empty.
Displays the final count of blocks in the blockchain and any relevant statistics or insights derived from the mining process.

Throughout the simulation, the program provides real-time feedback and outputs, such as the validation status of transactions and the addition of new blocks, offering a detailed and interactive glimpse into the mechanics of blockchain technology.

### UTXO implementatio:
---
- **UTXO Structure**: Defined within user.h, a UTXO is a structure that contains the amount of currency and an identifier (utxoID), which is a unique hash generated based on the owner's details and the amount.
- **UTXO Association with Users**: Each user, represented by the User class, has a list (vector) of UTXOs. This list represents the user's available balance, where each UTXO is a discrete unit of currency that the user can spend.
- **Transaction Processing**:
When a transaction occurs, the program selects appropriate UTXOs from the sender's account equal to or exceeding the transaction amount.
The chosen UTXOs are then marked as spent and removed from the sender's list. If the total UTXO value exceeds the transaction amount, a new UTXO is created representing the ***change*** and is added back to the sender's UTXO list.
- **New UTXO Creation for Recipients**: Upon successful transaction validation, a new UTXO is created corresponding to the transaction amount and is added to the receiver's UTXO list, increasing their balance.
- **Balance Calculation**: A user's total balance is dynamically calculated as the sum of all their unspent UTXOs. This calculation happens whenever there's a need to display the user's balance or during transaction validation.

#### Example
---

From this image we can see that ***user0*** was sent 50728.46 coins. The color green shows user's UTXOs that have not been spent and stayed in the wallet and color yellow - received UTXOs. <br>

---
![received_utxo](https://github.com/gabskir/VU-simplified-blockchain/blob/main/Sent%20UTXO.png?raw=true).

---

Sender - ***user2***. The color green shows user's UTXOs that have not been spent and stayed in the wallet, red - spent UTXOs, yellow - received UTXOs, purple - change that was sent back to the sender. <br>

---
![removed_utxo](https://github.com/gabskir/VU-simplified-blockchain/blob/main/UTXO%20removal,%20change%20calculation.png?raw=true).

---

From these examples we can see the main logic of UTXO implementation. <br>
- The amount user2 wanted to send was 50728.46.
- The closest value UTXO was **UTXO 6e16018ede286ab649406ed3200a63f3cb991afab7f66fa8f69dc28ee0e7ac9a - 48919.95**, but because it was less than the amoun user wanted to send, other UTXO was added: **UTXO     f99b811f257b0d0e812935c5c3bea9dc4306402ac5948df83674d89b5738daf3 - 69115.25**.
- So user2 sent two UTXOs with the value equal to 118035.2. User0 got the amount sender wanted to send, and the remaining amount - change - was sent back to the user:
```
change = 118035.2 - 50728.46
change = 67306.74

```
As we can see from the image new UTXO that has the value of change was created and added to sender's wallet:
```
04823753fe9effd1a21a3eb3386e93d81f015fb5c2983c0b61013644f82f7102 - 67306.74
```

## V0.2.

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




