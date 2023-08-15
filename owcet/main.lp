/* IPET system */
max: 60 e3_8_binary_search + 20 e4_7_binary_search + 20 e1_6_binary_search + 20 e5_6_binary_search + 20 e7_6_binary_search + 20 e8_6_binary_search + 20 e4_5_binary_search + 30 e3_4_binary_search + 55 e6_3_binary_search + 20 e6_2_binary_search + 55 e0_1_binary_search  + 10 e3_2_main + 25 e0_1_main;
e5_6_binary_search + e7_6_binary_search + e8_6_binary_search <= 5 e1_6_binary_search;	/* loop constraint on BB 6 (00008260)*/
x9_binary_search = e2_9_binary_search;	/* structural input constraint*/
x8_binary_search = e8_6_binary_search;	/* structural output constraint*/
x8_binary_search = e3_8_binary_search;	/* structural input constraint*/
x7_binary_search = e7_6_binary_search;	/* structural output constraint*/
x7_binary_search = e4_7_binary_search;	/* structural input constraint*/
x6_binary_search = e6_3_binary_search + e6_2_binary_search;	/* structural output constraint*/
x6_binary_search = e1_6_binary_search + e5_6_binary_search + e7_6_binary_search + e8_6_binary_search;	/* structural input constraint*/
x5_binary_search = e5_6_binary_search;	/* structural output constraint*/
x5_binary_search = e4_5_binary_search;	/* structural input constraint*/
x4_binary_search = e4_7_binary_search + e4_5_binary_search;	/* structural output constraint*/
x4_binary_search = e3_4_binary_search;	/* structural input constraint*/
x3_binary_search = e3_8_binary_search + e3_4_binary_search;	/* structural output constraint*/
x3_binary_search = e6_3_binary_search;	/* structural input constraint*/
x2_binary_search = e2_9_binary_search;	/* structural output constraint*/
x2_binary_search = e6_2_binary_search;	/* structural input constraint*/
x1_binary_search = e1_6_binary_search;	/* structural output constraint*/
x1_binary_search = e0_1_binary_search;	/* structural input constraint*/
x0_binary_search = e0_1_binary_search;	/* structural output constraint*/
x0_binary_search = x3_main;	/* call constraint*/
x4_main = e2_4_main;	/* structural input constraint*/
x3_main = e3_2_main;	/* structural output constraint*/
x3_main = e1_3_main;	/* structural input constraint*/
x2_main = e2_4_main;	/* structural output constraint*/
x2_main = e3_2_main;	/* structural input constraint*/
x1_main = e1_3_main;	/* structural output constraint*/
x1_main = e0_1_main;	/* structural input constraint*/
x0_main = e0_1_main;	/* structural output constraint*/
x0_main = 1;	/* program entry constraint*/
int x1_main;
int e2_4_main;
int x3_main;
int e3_2_main;
int x4_main;
int x0_binary_search;
int e0_1_binary_search;
int x1_binary_search;
int e1_6_binary_search;
int x2_binary_search;
int e2_9_binary_search;
int x3_binary_search;
int e3_4_binary_search;
int e3_8_binary_search;
int x4_binary_search;
int e4_5_binary_search;
int e4_7_binary_search;
int x5_binary_search;
int e5_6_binary_search;
int x6_binary_search;
int e6_2_binary_search;
int e6_3_binary_search;
int x7_binary_search;
int e7_6_binary_search;
int x8_binary_search;
int e8_6_binary_search;
int x9_binary_search;
int x2_main;
int e1_3_main;
int x0_main;
int e0_1_main;
