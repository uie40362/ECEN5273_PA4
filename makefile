final:
	gcc -o DFC/dfc DFC/dfc.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs1 DFS/dfs1.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs2 DFS/dfs2.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs3 DFS/dfs3.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs4 DFS/dfs4.c -pthread -lcrypto -lssl
