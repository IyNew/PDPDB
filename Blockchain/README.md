Clone the Fabric repository from GitHub

`curl -sSL https://bit.ly/2ysbOFE | bash -s`

Put the scripts under the Fabric directory. 

Initialize the couchdb database

`docker run -p 5984:5984 couchdb:2`

Open 4 terminals and run the scripts 1 to 4 in each. `source ./config` before every script.

Put the graph sample data in the form of json structure to be read by the application.