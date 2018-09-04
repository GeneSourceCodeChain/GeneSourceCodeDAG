# GeneSourceCodeDAG

The Gene Source Code Foundation is committed to build the world's first storage
and decentralized application platform for genomic and health data, the Gene Source Code System, with the expectation that a human healthcare data ecosystem will be built around this and create one unique Economic Mechanism: Encourage users to share genomic and health data within the ecosystem, provide researchers with adequate data support and provide data users with a friendly interface.

This project is to implement a library managing DAG objects such as block chain, merkle tree and merkle-patricia tree for Gene Source Code Chain. 

### Introduction

This project is going to proceed in triple steps:

1. first phase: We investigate several existing blockchain systems. We choose one among them on which we build Gene Source Code Chain 1.0.
2. second phase: We do modifications including decentralized storage, proof of storage and a better consensus algorithm to the chosed blockchain to enable genomic data storage and sharing.
3. last phase: We are going to independently implement a novel blockchain system to serve the Gene Source Code Chain ecosystem.

We are in the second phase in which we modify EOS.IO to get Gene Source Code Chain 2.0. The following sub projects are what we are doing.

1) EOS.IO wrapper (EOSWrapper)

Wrapper library which controls the EOS.IO toolkits. The demo provide an example of how to start a multi-node testnet.

2) EOS launcher scripts (EOSDeployer)

scripts for launching EOS testnet in one command

3) ETH launcher scripts (ETHDeployer)

scripts for launching ETH testnet easily


