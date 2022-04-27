# ECEN5273 PA4 (DFC/DFS)
## This code is incomplete as I did not have enough time to finish the assignment

### Instructions
1. Unpack the tarball
2. Run the makefile with the `make` command
3. To run the DFC `cd` to the `DFC` directory then simply type the command `./dfc`
4. The DFC connects based on the DFS IP and port set in `dfc.conf`
### Explanations
1. The DFC first parses the DFS IP and port set in `dfc.conf`
2. The DFC then connects to the four DFSes
3. The DFC then parses the command input by the user
4. #### put command
   1. **Only the put command was done but even that is not functional**
   2. When the DFC receives the put command it parses to get the filename
   3. The filename is then checked to see if it exists in the system
   4. The DFC then authenticates the user and password set in `dfc.conf` with DFS1
   5. The put command is sent to all four DFSes
   6. the md5sum for the desired file is calculated
   7. We then perform `md5sum % 4` to get the key
   8. We split the desired file into 4 parts
   9. With the key and filename we can set four `dfs structs` that contain the details necessary to send to each dfs
   10. Finally send the corresponding file parts to each dfs