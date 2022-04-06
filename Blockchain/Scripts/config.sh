
name="Searchtest"
version="1.0"
path_to_excutables="../Applications/"$name"/src/"$name
channelID="ch"$version
txID=$channelID".tx"
blockID=$channelID".block"
testfilepath="../json/"$name".json"

export PATH=$(pwd)/build/bin:$PATH 
export FABRIC_CFG_PATH=$(pwd)/sampleconfig 


# alias init='CORE_PEER_ADDRESS=127.0.0.1:7051 peer chaincode invoke -o 127.0.0.1:7050 -C $channelID -n $name -c '\'{\"Args\":\[\"InitLedger\"\]}\'' --isInit'

# alias secondinit='CORE_PEER_ADDRESS=127.0.0.1:7051 peer chaincode invoke -o 127.0.0.1:7050 -C $channelID -n $name -c '\'{\"Args\":\[\"SecondInit\"\,\"2\"\]}\'''

# alias test='CORE_PEER_ADDRESS=127.0.0.1:7051 peer chaincode invoke -o 127.0.0.1:7050 -C $channelID -n $name -c '\'{\"Args\":\[\"SearchWithDifferentTag\"\]}\'''
