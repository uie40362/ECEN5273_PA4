final:
	gcc -o DFC/dfc DFC/dfc.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs11 DFS/dfs1.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs22 DFS/dfs2.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs33 DFS/dfs3.c -pthread -lcrypto -lssl
	gcc -o DFS/dfs44 DFS/dfs4.c -pthread -lcrypto -lssl
