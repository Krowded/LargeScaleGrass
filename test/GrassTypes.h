#define vertNum 32

////Index array
GLuint numOfIndices = (3*vertNum)-6;
GLuint indices[] = 
{
	0, 1, 2,
	1, 3, 2,

	2, 3, 4,
	3, 5, 4,

	4, 5, 6,
	5, 7, 6,

	6, 7, 8,
	7, 9, 8,

	8, 9, 10,
	9, 11, 10,

	10, 11, 12,
	11, 13, 12,

	12, 13, 14,
	13, 15, 14,

	14, 15, 16,
	15, 17, 16,

	16, 17, 18,
	17, 19, 18,

	18, 19, 20,
	19, 21, 20,

	20, 21, 22,
	21, 23, 22,

	22, 23, 24,
	23, 25, 24,

	24, 25, 26,
	25, 27, 26,

	26, 27, 28,
	27, 29, 28,

	28, 29, 30,
	29, 31, 30,
};

GLuint combinedIndices[] = 
{
	0, 1, 2,
	1, 3, 2,

	2, 3, 4,
	3, 5, 4,

	4, 5, 6,
	5, 7, 6,

	6, 7, 8,
	7, 9, 8,

	8, 9, 10,
	9, 11, 10,

	10, 11, 12,
	11, 13, 12,

	12, 13, 14,
	13, 15, 14,

	14, 15, 16,
	15, 17, 16,

	16, 17, 18,
	17, 19, 18,

	18, 19, 20,
	19, 21, 20,

	20, 21, 22,
	21, 23, 22,

	22, 23, 24,
	23, 25, 24,

	24, 25, 26,
	25, 27, 26,

	26, 27, 28,
	27, 29, 28,

	28, 29, 30,
	29, 31, 30,

	32, 33, 34,
	33, 35, 34,

	34, 35, 36,
	35, 37, 36,

	36, 37, 38,
	37, 39, 38,

	38, 39, 40,
	39, 41, 40,

	40, 41, 42,
	41, 43, 42,

	42, 43, 44,
	43, 45, 44,

	44, 45, 46,
	45, 47, 46,

	46, 47, 48,
	47, 49, 48,

	48, 49, 50,
	49, 51, 50,

	50, 51, 52,
	51, 53, 52,

	52, 53, 54,
	53, 55, 54,

	54, 55, 56,
	55, 57, 56,

	56, 57, 58,
	57, 59, 58,

	58, 59, 60,
	59, 61, 60,

	60, 61, 62,
	61, 63, 62,

	64, 65, 66,
	65, 67, 66,

	66, 67, 68,
	67, 69, 68,

	68, 69, 70,
	69, 71, 70,

	70, 71, 72,
	71, 73, 72,

	72, 73, 74,
	73, 75, 74,

	74, 75, 76,
	75, 77, 76,

	76, 77, 78,
	77, 79, 78,

	78, 79, 80,
	79, 81, 80,

	80, 81, 82,
	81, 83, 82,

	82, 83, 84,
	83, 85, 84,

	84, 85, 86,
	85, 87, 86,

	86, 87, 88,
	87, 89, 88,

	88, 89, 90,
	89, 91, 90,

	90, 91, 92,
	91, 93, 92,

	92, 93, 94,
	93, 95, 94,

	96, 97, 98,
	97, 99, 98,

	98, 99, 100,
	99, 101, 100,

	100, 101, 102,
	101, 103, 102,

	102, 103, 104,
	103, 105, 104,

	104, 105, 106,
	105, 107, 106,

	106, 107, 108,
	107, 109, 108,

	108, 109, 110,
	109, 111, 110,

	110, 111, 112,
	111, 113, 112,

	112, 113, 114,
	113, 115, 114,

	114, 115, 116,
	115, 117, 116,

	116, 117, 118,
	117, 119, 118,

	118, 119, 120,
	119, 121, 120,

	120, 121, 122,
	121, 123, 122,

	122, 123, 124,
	123, 125, 124,

	124, 125, 126,
	125, 127, 126,

	128, 129, 130,
	129, 131, 130,

	130, 131, 132,
	131, 133, 132,

	132, 133, 134,
	133, 135, 134,

	134, 135, 136,
	135, 137, 136,

	136, 137, 138,
	137, 139, 138,

	138, 139, 140,
	139, 141, 140,

	140, 141, 142,
	141, 143, 142,

	142, 143, 144,
	143, 145, 144,

	144, 145, 146,
	145, 147, 146,

	146, 147, 148,
	147, 149, 148,

	148, 149, 150,
	149, 151, 150,

	150, 151, 152,
	151, 153, 152,

	152, 153, 154,
	153, 155, 154,

	154, 155, 156,
	155, 157, 156,

	156, 157, 158,
	157, 159, 158,
};

////////////Grass types
GLfloat Zero[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,


	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,
	
	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,		
	
	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,

	0.0f,0.0f,
	0.0f,0.0f,
};

GLfloat Type1[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	-0.19f,0.89f,
	-0.19f,0.89f,

	-0.23f,1.83f,
	-0.23f,1.83f,

	-0.16f,2.74f,
	-0.16f,2.74f,

	-0.09f,3.64f,
	-0.09f,3.64f,
	
	0.08f,4.51f,
	0.08f,4.51f,

	0.31f,5.24f,
	0.31f,5.24f,

	0.55f,5.98f,
	0.55f,5.98f,

	0.85f,6.59f,
	0.85f,6.59f,

	1.17f,6.98f,
	1.17f,6.98f,

	1.5f,7.37f,
	1.5f,7.37f,

	1.84f,7.55f,
	1.84f,7.55f,

	2.16f,7.41f,
	2.16f,7.41f,

	2.49f,7.28f,
	2.49f,7.28f,

	2.79f,6.83f,
	2.79f,6.83f,

	3.03f,5.99f,
	3.03f,5.99f,
};

GLfloat Type2[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.01f,0.81f,
	0.01f,0.81f,

	0.09f,1.51f,
	0.09f,1.51f,

	0.21f,2.11f,
	0.21f,2.11f,

	0.33f,2.72f,
	0.33f,2.72f,
	
	0.5f,3.22f,
	0.5f,3.22f,

	0.71f,3.66f,
	0.71f,3.66f,

	0.91f,4.09f,
	0.91f,4.09f,

	1.14f,4.44f,
	1.14f,4.44f,

	1.39f,4.74f,
	1.39f,4.74f,

	1.63f,5.04f,
	1.63f,5.04f,

	1.89f,5.27f,
	1.89f,5.27f,

	2.14f,5.47f,
	2.14f,5.47f,

	2.4f,5.67f,
	2.4f,5.67f,

	2.64f,5.82f,
	2.64f,5.82f,

	2.87f,5.95f,
	2.87f,5.95f,
};

GLfloat Type3[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.08f,0.75f,
	0.08f,0.75f,

	0.31f,1.35f,
	0.31f,1.35f,

	0.62f,1.82f,
	0.62f,1.82f,

	0.92f,2.28f,
	0.92f,2.28f,
	
	1.31f,2.61f,
	1.31f,2.61f,

	1.69f,2.83f,
	1.69f,2.83f,

	2.08f,3.04f,
	2.08f,3.04f,

	2.46f,3.12f,
	2.46f,3.12f,

	2.78f,3.11f,
	2.78f,3.11f,

	3.09f,3.09f,
	3.09f,3.09f,

	3.33f,2.96f,
	3.33f,2.96f,

	3.43f,2.74f,
	3.43f,2.74f,

	3.52f,2.52f,
	3.52f,2.52f,

	3.47f,2.21f,
	3.47f,2.21f,

	3.2f,1.82f,
	3.2f,1.82f,
};


GLfloat Type4[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.16f,0.77f,
	0.16f,0.77f,

	0.29f,1.41f,
	0.29f,1.41f,

	0.39f,1.94f,
	0.39f,1.94f,

	0.49f,2.48f,
	0.49f,2.48f,
	
	0.58f,2.91f,
	0.58f,2.91f,

	0.66f,3.26f,
	0.66f,3.26f,

	0.75f,3.61f,
	0.75f,3.61f,

	0.84f,3.87f,
	0.84f,3.87f,

	0.96f,4.09f,
	0.96f,4.09f,

	1.07f,4.3f,
	1.07f,4.3f,

	1.21f,4.46f,
	1.21f,4.46f,

	1.4f,4.58f,
	1.4f,4.58f,

	1.59f,4.7f,
	1.59f,4.7f,

	1.82f,4.8f,
	1.82f,4.8f,

	2.13f,4.88f,
	2.13f,4.88f,
};

GLfloat Type5[] =
{
	0.0f,0.0f,
	0.0f,0.0f,
	-0.24f,0.23f,
	-0.24f,0.23f,

	-0.41f,0.45f,
	-0.41f,0.45f,

	-0.49f,0.66f,
	-0.49f,0.66f,

	-0.58f,0.86f,
	-0.58f,0.86f,
	
	-0.58f,1.05f,
	-0.58f,1.05f,

	-0.5f,1.24f,
	-0.5f,1.24f,

	-0.42f,1.43f,
	-0.42f,1.43f,

	-0.26f,1.62f,
	-0.26f,1.62f,

	-0.02f,1.8f,
	-0.02f,1.8f,

	0.22f,1.99f,
	0.22f,1.99f,

	0.55f,2.17f,
	0.55f,2.17f,

	0.95f,2.37f,
	0.95f,2.37f,

	1.36f,2.56f,
	1.36f,2.56f,

	1.85f,2.76f,
	1.85f,2.76f,

	2.43f,2.98f,
	2.43f,2.98f,
};

GLfloat combinedVertices[] = {
	0.0f,0.0f,
	0.0f,0.0f,
	-0.19f,0.89f,
	-0.19f,0.89f,

	-0.23f,1.83f,
	-0.23f,1.83f,

	-0.16f,2.74f,
	-0.16f,2.74f,

	-0.09f,3.64f,
	-0.09f,3.64f,
	
	0.08f,4.51f,
	0.08f,4.51f,

	0.31f,5.24f,
	0.31f,5.24f,

	0.55f,5.98f,
	0.55f,5.98f,

	0.85f,6.59f,
	0.85f,6.59f,

	1.17f,6.98f,
	1.17f,6.98f,

	1.5f,7.37f,
	1.5f,7.37f,

	1.84f,7.55f,
	1.84f,7.55f,

	2.16f,7.41f,
	2.16f,7.41f,

	2.49f,7.28f,
	2.49f,7.28f,

	2.79f,6.83f,
	2.79f,6.83f,

	3.03f,5.99f,
	3.03f,5.99f,

	0.0f,0.0f,
	0.0f,0.0f,
	0.01f,0.81f,
	0.01f,0.81f,

	0.09f,1.51f,
	0.09f,1.51f,

	0.21f,2.11f,
	0.21f,2.11f,

	0.33f,2.72f,
	0.33f,2.72f,
	
	0.5f,3.22f,
	0.5f,3.22f,

	0.71f,3.66f,
	0.71f,3.66f,

	0.91f,4.09f,
	0.91f,4.09f,

	1.14f,4.44f,
	1.14f,4.44f,

	1.39f,4.74f,
	1.39f,4.74f,

	1.63f,5.04f,
	1.63f,5.04f,

	1.89f,5.27f,
	1.89f,5.27f,

	2.14f,5.47f,
	2.14f,5.47f,

	2.4f,5.67f,
	2.4f,5.67f,

	2.64f,5.82f,
	2.64f,5.82f,

	2.87f,5.95f,
	2.87f,5.95f,

	0.0f,0.0f,
	0.0f,0.0f,
	0.08f,0.75f,
	0.08f,0.75f,

	0.31f,1.35f,
	0.31f,1.35f,

	0.62f,1.82f,
	0.62f,1.82f,

	0.92f,2.28f,
	0.92f,2.28f,
	
	1.31f,2.61f,
	1.31f,2.61f,

	1.69f,2.83f,
	1.69f,2.83f,

	2.08f,3.04f,
	2.08f,3.04f,

	2.46f,3.12f,
	2.46f,3.12f,

	2.78f,3.11f,
	2.78f,3.11f,

	3.09f,3.09f,
	3.09f,3.09f,

	3.33f,2.96f,
	3.33f,2.96f,

	3.43f,2.74f,
	3.43f,2.74f,

	3.52f,2.52f,
	3.52f,2.52f,

	3.47f,2.21f,
	3.47f,2.21f,

	3.2f,1.82f,
	3.2f,1.82f,

	0.0f,0.0f,
	0.0f,0.0f,
	0.16f,0.77f,
	0.16f,0.77f,

	0.29f,1.41f,
	0.29f,1.41f,

	0.39f,1.94f,
	0.39f,1.94f,

	0.49f,2.48f,
	0.49f,2.48f,
	
	0.58f,2.91f,
	0.58f,2.91f,

	0.66f,3.26f,
	0.66f,3.26f,

	0.75f,3.61f,
	0.75f,3.61f,

	0.84f,3.87f,
	0.84f,3.87f,

	0.96f,4.09f,
	0.96f,4.09f,

	1.07f,4.3f,
	1.07f,4.3f,

	1.21f,4.46f,
	1.21f,4.46f,

	1.4f,4.58f,
	1.4f,4.58f,

	1.59f,4.7f,
	1.59f,4.7f,

	1.82f,4.8f,
	1.82f,4.8f,

	2.13f,4.88f,
	2.13f,4.88f,

	0.0f,0.0f,
	0.0f,0.0f,
	-0.24f,0.23f,
	-0.24f,0.23f,

	-0.41f,0.45f,
	-0.41f,0.45f,

	-0.49f,0.66f,
	-0.49f,0.66f,

	-0.58f,0.86f,
	-0.58f,0.86f,
	
	-0.58f,1.05f,
	-0.58f,1.05f,

	-0.5f,1.24f,
	-0.5f,1.24f,

	-0.42f,1.43f,
	-0.42f,1.43f,

	-0.26f,1.62f,
	-0.26f,1.62f,

	-0.02f,1.8f,
	-0.02f,1.8f,

	0.22f,1.99f,
	0.22f,1.99f,

	0.55f,2.17f,
	0.55f,2.17f,

	0.95f,2.37f,
	0.95f,2.37f,

	1.36f,2.56f,
	1.36f,2.56f,

	1.85f,2.76f,
	1.85f,2.76f,

	2.43f,2.98f,
	2.43f,2.98f,
};

//////Normals

GLfloat Type1Normals[] = {
	-1.0f, 0.0f,
	-1.0f, 0.0f,
	
	-0.992019f, -0.12609f, 
	-0.992019f, -0.12609f, 
	
	-0.999853f, 0.0171213f,
	-0.999853f, 0.0171213f,
	
	-0.997022f, 0.07712f, 
	-0.997022f, 0.07712f, 
	
	-0.990861f, 0.134884f, 
	-0.990861f, 0.134884f, 
	
	-0.969135f, 0.246529f, 
	-0.969135f, 0.246529f, 
	
	-0.95251f, 0.304508f, 
	-0.95251f, 0.304508f, 

	-0.926669f, 0.375879f, 
	-0.926669f, 0.375879f, 

	-0.840768f, 0.541396f, 
	-0.840768f, 0.541396f, 

	-0.768252f, 0.640148f, 
	-0.768252f, 0.640148f, 

	-0.627018f, 0.779005f, 
	-0.627018f, 0.779005f, 

	-0.0372357f, 0.999306f,
	-0.0372357f, 0.999306f,

	0.383738f, 0.923442f,
	0.383738f, 0.923442f,
	
	0.62804f, 0.778181f, 
	0.62804f, 0.778181f, 

	0.907648f, 0.419733f, 
	0.907648f, 0.419733f, 
	
	0.892332f, -0.45138f, 
	0.892332f, -0.45138f, 
};

GLfloat Type2Normals[] = {
	-1.0f, 0.0f, 
	-1.0f, 0.0f, 

	-0.998012f, 0.0630267f,
	-0.998012f, 0.0630267f,

	-0.98792f, 0.154967f, 
	-0.98792f, 0.154967f, 

	-0.980889f, 0.194569f,
	-0.980889f, 0.194569f,

	-0.966135f, 0.258037f,
	-0.966135f, 0.258037f,

	-0.926227f, 0.376967f,
	-0.926227f, 0.376967f,

	-0.904612f, 0.426235f,
	-0.904612f, 0.426235f,

	-0.873541f, 0.486751f,
	-0.873541f, 0.486751f,

	-0.803253f, 0.595638f,
	-0.803253f, 0.595638f,

	-0.774584f, 0.632471f,
	-0.774584f, 0.632471f,

	-0.724393f, 0.689388f,
	-0.724393f, 0.689388f,

	-0.643831f, 0.765167f,
	-0.643831f, 0.765167f,

	-0.617231f, 0.786782f,
	-0.617231f, 0.786782f,
	
	-0.570527f, 0.821279f,
	-0.570527f, 0.821279f,
	
	-0.511152f, 0.85949f, 
	-0.511152f, 0.85949f, 
	
	0.900695f, -0.434453f,
	0.900695f, -0.434453f,
};


GLfloat Type3Normals[] = {
	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.972243f, 0.233972f,
	-0.972243f, 0.233972f,

	-0.889491f, 0.456953f,
	-0.889491f, 0.456953f,

	-0.836194f, 0.548433f,
	-0.836194f, 0.548433f,

	-0.749678f, 0.661803f,
	-0.749678f, 0.661803f,
	
	-0.575754f, 0.817623f,
	-0.575754f, 0.817623f,

	-0.487626f, 0.873053f,
	-0.487626f, 0.873053f,

	-0.343573f, 0.939126f,
	-0.343573f, 0.939126f,

	-0.088014f, 0.996119f,
	-0.088014f, 0.996119f,

	0.0478151f, 0.998856f,
	0.0478151f, 0.998856f,

	0.276764f, 0.960938f, 
	0.276764f, 0.960938f, 

	0.731347f, 0.682005f, 
	0.731347f, 0.682005f, 

	0.918125f, 0.396291f, 
	0.918125f, 0.396291f, 

	0.993486f, 0.113955f, 
	0.993486f, 0.113955f, 

	0.927649f, -0.373453f,
	0.927649f, -0.373453f,

	0.494383f, -0.869244f,
	0.494383f, -0.869244f,
};

GLfloat Type4Normals[] = {
	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.979539f, 0.201254f,
	-0.979539f, 0.201254f,

	-0.981348f, 0.192239f,
	-0.981348f, 0.192239f,

	-0.982973f, 0.183749f,
	-0.982973f, 0.183749f,

	-0.981102f, 0.193489f,
	-0.981102f, 0.193489f,

	-0.976866f, 0.213853f,
	-0.976866f, 0.213853f,

	-0.971764f, 0.235954f,
	-0.971764f, 0.235954f,

	-0.957536f, 0.288315f,
	-0.957536f, 0.288315f,

	-0.914593f, 0.404375f,
	-0.914593f, 0.404375f,

	-0.881895f, 0.471446f,
	-0.881895f, 0.471446f,

	-0.824956f, 0.565197f,
	-0.824956f, 0.565197f,

	-0.650043f, 0.759897f,
	-0.650043f, 0.759897f,

	-0.533993f, 0.845489f,
	-0.533993f, 0.845489f,

	-0.467731f, 0.883871f,
	-0.467731f, 0.883871f,

	-0.325312f, 0.945607f,
	-0.325312f, 0.945607f,

	0.916502f -0.400031f, 
	0.916502f -0.400031f, 
};

GLfloat Type5Normals[] = {
	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.743652f, -0.668567f,
	-0.743652f, -0.668567f,

	-0.872288f, -0.488992f,
	-0.872288f, -0.488992f,

	-0.923605f, -0.383346f,
	-0.923605f, -0.383346f,
	
	-0.977732f, -0.209855f,
	-0.977732f, -0.209855f,

	-0.980213f, 0.197945f, 
	-0.980213f, 0.197945f, 

	-0.921635f, 0.388057f, 
	-0.921635f, 0.388057f, 

	-0.852938f, 0.522012f, 
	-0.852938f, 0.522012f, 

	-0.686891f, 0.726761f, 
	-0.686891f, 0.726761f, 

	-0.610404f, 0.792091f, 
	-0.610404f, 0.792091f, 

	-0.551777f, 0.833992f, 
	-0.551777f, 0.833992f, 

	-0.463107f, 0.886303f, 
	-0.463107f, 0.886303f, 

	-0.433885f, 0.900968f, 
	-0.433885f, 0.900968f, 

	-0.399287f, 0.916826f, 
	-0.399287f, 0.916826f, 
	
	-0.366304f, 0.930495f, 
	-0.366304f, 0.930495f, 

	0.655852f, -0.75489f,
	0.655852f, -0.75489f,
};

GLfloat combinedNormals[] = {
	-1.0f, 0.0f,
	-1.0f, 0.0f,
	
	-0.992019f, -0.12609f, 
	-0.992019f, -0.12609f, 
	
	-0.999853f, 0.0171213f,
	-0.999853f, 0.0171213f,
	
	-0.997022f, 0.07712f, 
	-0.997022f, 0.07712f, 
	
	-0.990861f, 0.134884f, 
	-0.990861f, 0.134884f, 
	
	-0.969135f, 0.246529f, 
	-0.969135f, 0.246529f, 
	
	-0.95251f, 0.304508f, 
	-0.95251f, 0.304508f, 

	-0.926669f, 0.375879f, 
	-0.926669f, 0.375879f, 

	-0.840768f, 0.541396f, 
	-0.840768f, 0.541396f, 

	-0.768252f, 0.640148f, 
	-0.768252f, 0.640148f, 

	-0.627018f, 0.779005f, 
	-0.627018f, 0.779005f, 

	-0.0372357f, 0.999306f,
	-0.0372357f, 0.999306f,

	0.383738f, 0.923442f,
	0.383738f, 0.923442f,
	
	0.62804f, 0.778181f, 
	0.62804f, 0.778181f, 

	0.907648f, 0.419733f, 
	0.907648f, 0.419733f, 
	
	0.892332f, -0.45138f, 
	0.892332f, -0.45138f, 

	-1.0f, 0.0f, 
	-1.0f, 0.0f, 

	-0.998012f, 0.0630267f,
	-0.998012f, 0.0630267f,

	-0.98792f, 0.154967f, 
	-0.98792f, 0.154967f, 

	-0.980889f, 0.194569f,
	-0.980889f, 0.194569f,

	-0.966135f, 0.258037f,
	-0.966135f, 0.258037f,

	-0.926227f, 0.376967f,
	-0.926227f, 0.376967f,

	-0.904612f, 0.426235f,
	-0.904612f, 0.426235f,

	-0.873541f, 0.486751f,
	-0.873541f, 0.486751f,

	-0.803253f, 0.595638f,
	-0.803253f, 0.595638f,

	-0.774584f, 0.632471f,
	-0.774584f, 0.632471f,

	-0.724393f, 0.689388f,
	-0.724393f, 0.689388f,

	-0.643831f, 0.765167f,
	-0.643831f, 0.765167f,

	-0.617231f, 0.786782f,
	-0.617231f, 0.786782f,
	
	-0.570527f, 0.821279f,
	-0.570527f, 0.821279f,
	
	-0.511152f, 0.85949f, 
	-0.511152f, 0.85949f, 
	
	0.900695f, -0.434453f,
	0.900695f, -0.434453f,

	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.972243f, 0.233972f,
	-0.972243f, 0.233972f,

	-0.889491f, 0.456953f,
	-0.889491f, 0.456953f,

	-0.836194f, 0.548433f,
	-0.836194f, 0.548433f,

	-0.749678f, 0.661803f,
	-0.749678f, 0.661803f,
	
	-0.575754f, 0.817623f,
	-0.575754f, 0.817623f,

	-0.487626f, 0.873053f,
	-0.487626f, 0.873053f,

	-0.343573f, 0.939126f,
	-0.343573f, 0.939126f,

	-0.088014f, 0.996119f,
	-0.088014f, 0.996119f,

	0.0478151f, 0.998856f,
	0.0478151f, 0.998856f,

	0.276764f, 0.960938f, 
	0.276764f, 0.960938f, 

	0.731347f, 0.682005f, 
	0.731347f, 0.682005f, 

	0.918125f, 0.396291f, 
	0.918125f, 0.396291f, 

	0.993486f, 0.113955f, 
	0.993486f, 0.113955f, 

	0.927649f, -0.373453f,
	0.927649f, -0.373453f,

	0.494383f, -0.869244f,
	0.494383f, -0.869244f,

	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.979539f, 0.201254f,
	-0.979539f, 0.201254f,

	-0.981348f, 0.192239f,
	-0.981348f, 0.192239f,

	-0.982973f, 0.183749f,
	-0.982973f, 0.183749f,

	-0.981102f, 0.193489f,
	-0.981102f, 0.193489f,

	-0.976866f, 0.213853f,
	-0.976866f, 0.213853f,

	-0.971764f, 0.235954f,
	-0.971764f, 0.235954f,

	-0.957536f, 0.288315f,
	-0.957536f, 0.288315f,

	-0.914593f, 0.404375f,
	-0.914593f, 0.404375f,

	-0.881895f, 0.471446f,
	-0.881895f, 0.471446f,

	-0.824956f, 0.565197f,
	-0.824956f, 0.565197f,

	-0.650043f, 0.759897f,
	-0.650043f, 0.759897f,

	-0.533993f, 0.845489f,
	-0.533993f, 0.845489f,

	-0.467731f, 0.883871f,
	-0.467731f, 0.883871f,

	-0.325312f, 0.945607f,
	-0.325312f, 0.945607f,

	0.916502f -0.400031f, 
	0.916502f -0.400031f, 

	-1.0f, 0.0f,
	-1.0f, 0.0f,

	-0.743652f, -0.668567f,
	-0.743652f, -0.668567f,

	-0.872288f, -0.488992f,
	-0.872288f, -0.488992f,

	-0.923605f, -0.383346f,
	-0.923605f, -0.383346f,
	
	-0.977732f, -0.209855f,
	-0.977732f, -0.209855f,

	-0.980213f, 0.197945f, 
	-0.980213f, 0.197945f, 

	-0.921635f, 0.388057f, 
	-0.921635f, 0.388057f, 

	-0.852938f, 0.522012f, 
	-0.852938f, 0.522012f, 

	-0.686891f, 0.726761f, 
	-0.686891f, 0.726761f, 

	-0.610404f, 0.792091f, 
	-0.610404f, 0.792091f, 

	-0.551777f, 0.833992f, 
	-0.551777f, 0.833992f, 

	-0.463107f, 0.886303f, 
	-0.463107f, 0.886303f, 

	-0.433885f, 0.900968f, 
	-0.433885f, 0.900968f, 

	-0.399287f, 0.916826f, 
	-0.399287f, 0.916826f, 
	
	-0.366304f, 0.930495f, 
	-0.366304f, 0.930495f, 

	0.655852f, -0.75489f,
	0.655852f, -0.75489f,
};

//Texture coordinates
const float width = 15.0f;

GLfloat textureCoordinates[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,

	0.0f, 1.0f/width,
	1.0f, 1.0f/width,

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	0.0f, 3.0f/width,
	1.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	0.0f, 5.0f/width,
	1.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	0.0f, 7.0f/width,
	1.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	0.0f, 9.0f/width,
	1.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	0.0f, 11.0f/width,
	1.0f, 11.0f/width,

	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	0.0f, 13.0f/width,
	1.0f, 13.0f/width,

	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	0.0f, 15.0f/width,
	1.0f, 15.0f/width,
};

GLfloat combinedTextureCoordinates[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f/width,
	0.0f, 1.0f/width,
	

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	1.0f, 3.0f/width,
	0.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	1.0f, 5.0f/width,
	0.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	1.0f, 7.0f/width,
	0.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	1.0f, 9.0f/width,
	0.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	1.0f, 11.0f/width,
	0.0f, 11.0f/width,
	
	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	1.0f, 13.0f/width,
	0.0f, 13.0f/width,
	
	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	1.0f, 15.0f/width,
	0.0f, 15.0f/width,

	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f/width,
	0.0f, 1.0f/width,
	

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	1.0f, 3.0f/width,
	0.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	1.0f, 5.0f/width,
	0.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	1.0f, 7.0f/width,
	0.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	1.0f, 9.0f/width,
	0.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	1.0f, 11.0f/width,
	0.0f, 11.0f/width,
	
	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	1.0f, 13.0f/width,
	0.0f, 13.0f/width,
	
	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	1.0f, 15.0f/width,
	0.0f, 15.0f/width,

	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f/width,
	0.0f, 1.0f/width,
	

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	1.0f, 3.0f/width,
	0.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	1.0f, 5.0f/width,
	0.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	1.0f, 7.0f/width,
	0.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	1.0f, 9.0f/width,
	0.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	1.0f, 11.0f/width,
	0.0f, 11.0f/width,
	
	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	1.0f, 13.0f/width,
	0.0f, 13.0f/width,
	
	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	1.0f, 15.0f/width,
	0.0f, 15.0f/width,

	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f/width,
	0.0f, 1.0f/width,
	

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	1.0f, 3.0f/width,
	0.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	1.0f, 5.0f/width,
	0.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	1.0f, 7.0f/width,
	0.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	1.0f, 9.0f/width,
	0.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	1.0f, 11.0f/width,
	0.0f, 11.0f/width,
	
	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	1.0f, 13.0f/width,
	0.0f, 13.0f/width,
	
	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	1.0f, 15.0f/width,
	0.0f, 15.0f/width,

	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f/width,
	0.0f, 1.0f/width,
	

	0.0f, 2.0f/width,
	1.0f, 2.0f/width,

	1.0f, 3.0f/width,
	0.0f, 3.0f/width,

	0.0f, 4.0f/width,
	1.0f, 4.0f/width,
	
	1.0f, 5.0f/width,
	0.0f, 5.0f/width,

	0.0f, 6.0f/width,
	1.0f, 6.0f/width,

	1.0f, 7.0f/width,
	0.0f, 7.0f/width,

	0.0f, 8.0f/width,
	1.0f, 8.0f/width,

	1.0f, 9.0f/width,
	0.0f, 9.0f/width,

	0.0f, 10.0f/width,
	1.0f, 10.0f/width,

	1.0f, 11.0f/width,
	0.0f, 11.0f/width,
	
	0.0f, 12.0f/width,
	1.0f, 12.0f/width,

	1.0f, 13.0f/width,
	0.0f, 13.0f/width,
	
	0.0f, 14.0f/width,
	1.0f, 14.0f/width,

	1.0f, 15.0f/width,
	0.0f, 15.0f/width,
};
