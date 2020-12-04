
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <time.h>
#define NUM_NODES 34
#define NUM_EDGES 272

#define CUDACHECKERROR( call ) {			\
	cudaError_t result = call;              \
	if ( cudaSuccess != result )            \
    fprintf(stderr, "CUDA error %i in %s:%s\n%s", result, __FILE__, __LINE__, cudaGetErrorString( result ));  \
}

typedef struct
{
	int start;					// Starting index of edges
	int length;					// Number of edges
	char id[2] = {'0', '0'};	// Text ID of node, for display
} Node;

__global__ void Bfs_Kernel(Node* vertexList, int* edgeList, int* frontierList, bool* visitedList, int* distanceList, bool* complete, char* pathList)
{
	// Calculate vertix ID from threads/blocks
	int id = threadIdx.x;

	if (frontierList[id] == 1 && visitedList[id] == false)
	{
		// Check off node in visited array and frontier array
		visitedList[id] = true;
		frontierList[id] = 0;
		__syncthreads();

		// Calculate span of edges to process for the node
		int start = vertexList[id].start;
		int end = start + vertexList[id].length;
		
		// Process Edges
		for (int i = start; i < end; i++)
		{
			// Pull edge vertix id
			int childID = edgeList[i];
			
			// If neighbor vertex has not been visited
			if (visitedList[childID] == false && (atomicCAS(&frontierList[childID], 0, 1) != 1))
			{
				// Copy ID list to neighbor vertex
				for (int j = 0; j < NUM_NODES * 2; ++j) {
					pathList[childID * NUM_NODES * 2 + j] = pathList[id * NUM_NODES * 2 + j];
				}

				// Update vertex distance
				distanceList[childID] = distanceList[id] + 1;

				// Copy newest vertex ID
				pathList[childID * NUM_NODES * 2 + distanceList[childID]*2] = vertexList[id].id[0];
				pathList[childID * NUM_NODES * 2 + distanceList[childID]*2 + 1] = vertexList[id].id[1];

				// Set neighbor to true for frontier array
				*complete = false;
			}

		}

	}

}



int main(){

	// INIT GRAPH DATA
	Node node[NUM_NODES];
	int edges[NUM_EDGES];

	node[00].start = 0;
	node[00].length = 9;
	node[01].start = 9;
	node[01].length = 8;
	node[02].start = 17;
	node[02].length = 8;
	node[03].start = 25;
	node[03].length = 8;
	node[04].start = 33;
	node[04].length = 8;
	node[05].start = 41;
	node[05].length = 11;
	node[06].start = 52;
	node[06].length = 3;
	node[07].start = 55;
	node[07].length = 13;
	node[8].start = 68;
	node[8].length = 1;
	node[9].start = 69;
	node[9].length = 4;
	node[10].start = 73;
	node[10].length = 13;
	node[11].start = 86;
	node[11].length = 12;
	node[12].start = 98;
	node[12].length = 13;
	node[13].start = 111;
	node[13].length = 1;
	node[14].start = 112;
	node[14].length = 12;
	node[15].start = 124;
	node[15].length = 3;
	node[16].start = 127;
	node[16].length = 13;
	node[17].start = 140;
	node[17].length = 13;
	node[18].start = 153;
	node[18].length = 11;
	node[19].start = 164;
	node[19].length = 11;
	node[20].start = 175;
	node[20].length = 4;
	node[21].start = 179;
	node[21].length = 1;
	node[22].start = 180;
	node[22].length = 2;
	node[23].start = 182;
	node[23].length = 8;
	node[24].start = 190;
	node[24].length = 12;
	node[25].start = 202;
	node[25].length = 8;
	node[26].start = 210;
	node[26].length = 13;
	node[27].start = 223;
	node[27].length = 8;
	node[28].start = 231;
	node[28].length = 2;
	node[29].start = 233;
	node[29].length = 2;
	node[30].start = 235;
	node[30].length = 13;
	node[31].start = 248;
	node[31].length = 8;
	node[32].start = 256;
	node[32].length = 13;
	node[33].start = 269;
	node[33].length = 3;
	node[0].id[0] = '0'; node[0].id[1] = '0';
	node[1].id[0] = '0'; node[1].id[1] = '1';

	node[2].id[0] = '0'; node[2].id[1] = '2';

	node[3].id[0] = '0'; node[3].id[1] = '3';

	node[4].id[0] = '0'; node[4].id[1] = '4';

	node[5].id[0] = '0'; node[5].id[1] = '5';

	node[6].id[0] = '0'; node[6].id[1] = '6';

	node[7].id[0] = '0'; node[7].id[1] = '7';

	node[8].id[0] = '0'; node[8].id[1] = '8';

	node[9].id[0] = '0'; node[9].id[1] = '9';

	node[10].id[0] = '1'; node[10].id[1] = '0';

	node[11].id[0] = '1'; node[11].id[1] = '1';

	node[12].id[0] = '1'; node[12].id[1] = '2';

	node[13].id[0] = '1'; node[13].id[1] = '3';

	node[14].id[0] = '1'; node[14].id[1] = '4';

	node[15].id[0] = '1'; node[15].id[1] = '5';

	node[16].id[0] = '1'; node[16].id[1] = '6';

	node[17].id[0] = '1'; node[17].id[1] = '7';

	node[18].id[0] = '1'; node[18].id[1] = '8';

	node[19].id[0] = '1'; node[19].id[1] = '9';

	node[20].id[0] = '2'; node[20].id[1] = '0';

	node[21].id[0] = '2'; node[21].id[1] = '1';

	node[22].id[0] = '2'; node[22].id[1] = '2';

	node[23].id[0] = '2'; node[23].id[1] = '3';

	node[24].id[0] = '2'; node[24].id[1] = '4';

	node[25].id[0] = '2'; node[25].id[1] = '5';

	node[26].id[0] = '2'; node[26].id[1] = '6';

	node[27].id[0] = '2'; node[27].id[1] = '7';

	node[28].id[0] = '2'; node[28].id[1] = '8';

	node[29].id[0] = '2'; node[29].id[1] = '9';

	node[30].id[0] = '3'; node[30].id[1] = '0';

	node[31].id[0] = '3'; node[31].id[1] = '1';

	node[32].id[0] = '3'; node[32].id[1] = '2';

	node[33].id[0] = '3'; node[33].id[1] = '3';
	edges[0] = 01;
	edges[1] = 02;
	edges[2] = 03;
	edges[3] = 04;
	edges[4] = 23;
	edges[5] = 24;
	edges[6] = 25;
	edges[7] = 27;
	edges[8] = 31;
	edges[9] = 00;
	edges[10] = 02;
	edges[11] = 03;
	edges[12] = 04;
	edges[13] = 23;
	edges[14] = 25;
	edges[15] = 27;
	edges[16] = 31;
	edges[17] = 00;
	edges[18] = 01;
	edges[19] = 03;
	edges[20] = 04;
	edges[21] = 23;
	edges[22] = 24;
	edges[23] = 25;
	edges[24] = 31;
	edges[25] = 00;
	edges[26] = 01;
	edges[27] = 02;
	edges[28] = 04;
	edges[29] = 23;
	edges[30] = 25;
	edges[31] = 27;
	edges[32] = 31;
	edges[33] = 00;
	edges[34] = 01;
	edges[35] = 02;
	edges[36] = 03;
	edges[37] = 23;
	edges[38] = 25;
	edges[39] = 27;
	edges[40] = 31;
	edges[41] = 07;
	edges[42] = 10;
	edges[43] = 11;
	edges[44] = 12;
	edges[45] = 14;
	edges[46] = 16;
	edges[47] = 17;
	edges[48] = 24;
	edges[49] = 26;
	edges[50] = 30;
	edges[51] = 32;
	edges[52] = 9;
	edges[53] = 28;
	edges[54] = 29;
	edges[55] = 05;
	edges[56] = 10;
	edges[57] = 11;
	edges[58] = 12;
	edges[59] = 14;
	edges[60] = 16;
	edges[61] = 17;
	edges[62] = 18;
	edges[63] = 19;
	edges[64] = 24;
	edges[65] = 26;
	edges[66] = 30;
	edges[67] = 32;
	edges[68] = 13;
	edges[69] = 06;
	edges[70] = 15;
	edges[71] = 20;
	edges[72] = 33;
	edges[73] = 05;
	edges[74] = 07;
	edges[75] = 11;
	edges[76] = 12;
	edges[77] = 14;
	edges[78] = 16;
	edges[79] = 17;
	edges[80] = 18;
	edges[81] = 19;
	edges[82] = 24;
	edges[83] = 26;
	edges[84] = 30;
	edges[85] = 32;
	edges[86] = 05;
	edges[87] = 07;
	edges[88] = 10;
	edges[89] = 12;
	edges[90] = 14;
	edges[91] = 16;
	edges[92] = 17;
	edges[93] = 19;
	edges[94] = 24;
	edges[95] = 26;
	edges[96] = 30;
	edges[97] = 32;
	edges[98] = 05;
	edges[99] = 07;
	edges[100] = 10;
	edges[101] = 11;
	edges[102] = 14;
	edges[103] = 16;
	edges[104] = 17;
	edges[105] = 18;
	edges[106] = 19;
	edges[107] = 24;
	edges[108] = 26;
	edges[109] = 30;
	edges[110] = 32;
	edges[111] = 8;
	edges[112] = 05;
	edges[113] = 07;
	edges[114] = 10;
	edges[115] = 11;
	edges[116] = 12;
	edges[117] = 16;
	edges[118] = 17;
	edges[119] = 18;
	edges[120] = 19;
	edges[121] = 26;
	edges[122] = 30;
	edges[123] = 32;
	edges[124] = 9;
	edges[125] = 20;
	edges[126] = 33;
	edges[127] = 05;
	edges[128] = 07;
	edges[129] = 10;
	edges[130] = 11;
	edges[131] = 12;
	edges[132] = 14;
	edges[133] = 17;
	edges[134] = 18;
	edges[135] = 19;
	edges[136] = 24;
	edges[137] = 26;
	edges[138] = 30;
	edges[139] = 32;
	edges[140] = 05;
	edges[141] = 07;
	edges[142] = 10;
	edges[143] = 11;
	edges[144] = 12;
	edges[145] = 14;
	edges[146] = 16;
	edges[147] = 18;
	edges[148] = 19;
	edges[149] = 24;
	edges[150] = 26;
	edges[151] = 30;
	edges[152] = 32;
	edges[153] = 07;
	edges[154] = 10;
	edges[155] = 12;
	edges[156] = 14;
	edges[157] = 16;
	edges[158] = 17;
	edges[159] = 19;
	edges[160] = 20;
	edges[161] = 26;
	edges[162] = 30;
	edges[163] = 32;
	edges[164] = 07;
	edges[165] = 10;
	edges[166] = 11;
	edges[167] = 12;
	edges[168] = 14;
	edges[169] = 16;
	edges[170] = 17;
	edges[171] = 18;
	edges[172] = 26;
	edges[173] = 30;
	edges[174] = 32;
	edges[175] = 9;
	edges[176] = 15;
	edges[177] = 18;
	edges[178] = 33;
	edges[179] = 22;
	edges[180] = 21;
	edges[181] = 27;
	edges[182] = 00;
	edges[183] = 01;
	edges[184] = 02;
	edges[185] = 03;
	edges[186] = 04;
	edges[187] = 25;
	edges[188] = 27;
	edges[189] = 31;
	edges[190] = 00;
	edges[191] = 02;
	edges[192] = 05;
	edges[193] = 07;
	edges[194] = 10;
	edges[195] = 11;
	edges[196] = 12;
	edges[197] = 16;
	edges[198] = 17;
	edges[199] = 26;
	edges[200] = 30;
	edges[201] = 32;
	edges[202] = 00;
	edges[203] = 01;
	edges[204] = 02;
	edges[205] = 03;
	edges[206] = 04;
	edges[207] = 23;
	edges[208] = 27;
	edges[209] = 31;
	edges[210] = 05;
	edges[211] = 07;
	edges[212] = 10;
	edges[213] = 11;
	edges[214] = 12;
	edges[215] = 14;
	edges[216] = 16;
	edges[217] = 17;
	edges[218] = 18;
	edges[219] = 19;
	edges[220] = 24;
	edges[221] = 30;
	edges[222] = 32;
	edges[223] = 00;
	edges[224] = 01;
	edges[225] = 03;
	edges[226] = 04;
	edges[227] = 22;
	edges[228] = 23;
	edges[229] = 25;
	edges[230] = 31;
	edges[231] = 06;
	edges[232] = 29;
	edges[233] = 06;
	edges[234] = 28;
	edges[235] = 05;
	edges[236] = 07;
	edges[237] = 10;
	edges[238] = 11;
	edges[239] = 12;
	edges[240] = 14;
	edges[241] = 16;
	edges[242] = 17;
	edges[243] = 18;
	edges[244] = 19;
	edges[245] = 24;
	edges[246] = 26;
	edges[247] = 32;
	edges[248] = 00;
	edges[249] = 01;
	edges[250] = 02;
	edges[251] = 03;
	edges[252] = 04;
	edges[253] = 23;
	edges[254] = 25;
	edges[255] = 27;
	edges[256] = 05;
	edges[257] = 07;
	edges[258] = 10;
	edges[259] = 11;
	edges[260] = 12;
	edges[261] = 14;
	edges[262] = 16;
	edges[263] = 17;
	edges[264] = 18;
	edges[265] = 19;
	edges[266] = 24;
	edges[267] = 26;
	edges[268] = 30;
	edges[269] = 9;
	edges[270] = 15;
	edges[271] = 20;


	int frontier[NUM_NODES] = { 0 };
	int pathLength[NUM_NODES] = { 0 };
	bool visited[NUM_NODES] = { false };
	char path[NUM_NODES * NUM_NODES * 2] = { '0' };

	// Set source node as first to be visited
	int sourceIdx = 0;
	frontier[sourceIdx] = 1;

	// Init data on device
	Node* vertexList;
	int* edgeList;
	int* frontierList;
	bool* visitedList;
	int* distanceList;
	char* pathList;

	CUDACHECKERROR(cudaMalloc((void**)&vertexList, sizeof(Node) * NUM_NODES));
	CUDACHECKERROR(cudaMalloc((void**)&edgeList, sizeof(int) * NUM_EDGES));
	CUDACHECKERROR(cudaMalloc((void**)&frontierList, sizeof(int) * NUM_NODES));
	CUDACHECKERROR(cudaMalloc((void**)&visitedList, sizeof(bool) * NUM_NODES));
	CUDACHECKERROR(cudaMalloc((void**)&distanceList, sizeof(int) * NUM_NODES));
	CUDACHECKERROR(cudaMalloc((void**)&pathList, sizeof(char) * NUM_NODES * NUM_NODES * 2));

	CUDACHECKERROR(cudaMemcpy(vertexList, node, sizeof(Node) * NUM_NODES, cudaMemcpyHostToDevice));
	CUDACHECKERROR(cudaMemcpy(edgeList, edges, sizeof(int) * NUM_EDGES, cudaMemcpyHostToDevice));
	CUDACHECKERROR(cudaMemcpy(frontierList, frontier, sizeof(int) * NUM_NODES, cudaMemcpyHostToDevice));
	CUDACHECKERROR(cudaMemcpy(visitedList, visited, sizeof(bool) * NUM_NODES, cudaMemcpyHostToDevice));
	CUDACHECKERROR(cudaMemcpy(distanceList, pathLength, sizeof(int) * NUM_NODES, cudaMemcpyHostToDevice));
	CUDACHECKERROR(cudaMemcpy(pathList, path, sizeof(char) * NUM_NODES * NUM_NODES * 2, cudaMemcpyHostToDevice));

	// Prepare for loop
	bool complete;
	bool* completeDevice;
	CUDACHECKERROR(cudaMalloc((void**)&completeDevice, sizeof(bool)));
	int count = 0;

	// Setup timing
	float time;
	cudaEvent_t start, stop;

	// Start timer
	CUDACHECKERROR(cudaEventCreate(&start));
	CUDACHECKERROR(cudaEventCreate(&stop));
	CUDACHECKERROR(cudaEventRecord(start, 0));

	// Start BFS
	do {
		complete = true;
		CUDACHECKERROR(cudaMemcpy(completeDevice, &complete, sizeof(bool), cudaMemcpyHostToDevice));

		// While kernel not complete, loop until complete
		Bfs_Kernel << <1, NUM_NODES >> > (vertexList, edgeList, frontierList, visitedList, distanceList, completeDevice, pathList);

		// Get current result from device
		CUDACHECKERROR(cudaMemcpy(&complete, completeDevice, sizeof(bool), cudaMemcpyDeviceToHost));
		count++;

	} while (!complete);

	// End timer
	CUDACHECKERROR(cudaEventRecord(stop, 0));
	CUDACHECKERROR(cudaEventSynchronize(stop));
	CUDACHECKERROR(cudaEventElapsedTime(&time, start, stop));


	// Copy results to host
	CUDACHECKERROR(cudaMemcpy(pathLength, distanceList, sizeof(int) * NUM_NODES, cudaMemcpyDeviceToHost));
	CUDACHECKERROR(cudaMemcpy(path, pathList, sizeof(char) * NUM_NODES * NUM_NODES * 2, cudaMemcpyDeviceToHost));

	// print results
	printf("\n------\n");
	char a, b;
	for (int i = 0; i < NUM_NODES; ++i) {
		printf("%d:\t", i);
		for (int j = 0; j < NUM_NODES; ++j){
			a = path[i * NUM_NODES * 2 + j * 2];
			b = path[i * NUM_NODES * 2 + j * 2 + 1];
			if(a >= '0' && a <= '9' && b >= '0' && b <= '9')
				printf("%c%c,  ", a, b);
		}
		printf("\n");
	}
	printf("------\n");


	printf("\nTimes kernel was called: %d\n", count);
	printf("Elapsed Time:  %3.5f ms \n", time);


	CUDACHECKERROR(cudaFree(vertexList));
	CUDACHECKERROR(cudaFree(edgeList));
	CUDACHECKERROR(cudaFree(frontierList));
	CUDACHECKERROR(cudaFree(visitedList));
	CUDACHECKERROR(cudaFree(distanceList));
	CUDACHECKERROR(cudaFree(pathList));

}