final:
	gcc -o DFC/dfc DFC/dfc.c -pthread -lcrypto -lssl
	gcc -o DFS1/dfs1 DFS1/dfs1.c -pthread -lcrypto -lssl
	gcc -o DFS2/dfs2 DFS2/dfs2.c -pthread -lcrypto -lssl
	gcc -o DFS3/dfs3 DFS3/dfs3.c -pthread -lcrypto -lssl
	gcc -o DFS4/dfs4 DFS4/dfs4.c -pthread -lcrypto -lssl
