FILE *ranking_data;
FILE *ranking_nome;
//struct do tipo de dado score
struct score{
	char nome[4];
	double tempo;
	int posicao;
};

typedef struct score score;//definiÃ§Ã£o do tipo score

//faz tipo um boble sort
void addToRanking(score scoreToAdd, score *currentRanking, int n)
{
	int i;
	score aux;
	for (i = n; i < 100; i++)
	{
		//ve se na posiÃ§Ã£o da vez o rank tem nome = "...", que Ã© sem rank e adiciona o scoreToAdd nessa posiÃ§Ã£o em caso positivo
		if (!strcmp(currentRanking[i].nome, "..."))
		{
			strcpy(currentRanking[i].nome, scoreToAdd.nome);
			currentRanking[i].tempo = scoreToAdd.tempo;
			break;
		}
		//se o nome nÃ£o for "..." ele checa se o tempo Ã© maior
		else if (scoreToAdd.tempo < currentRanking[i].tempo)
		{
			//se for ele salva os valores antigos dessa posiÃ§Ã£o
			strcpy(aux.nome, currentRanking[i].nome);
			aux.tempo = currentRanking[i].tempo;
			aux.posicao = currentRanking[i].posicao;
			//adiciona os valores scoreToAdd nessa posiÃ§Ã£o
			strcpy(currentRanking[i].nome, scoreToAdd.nome);
			currentRanking[i].tempo = scoreToAdd.tempo;
			addToRanking(aux, currentRanking, i + 1);//chama a funÃ§Ã£o com os valores antigos dessa posiÃ§Ã£o
			break;
		}
	}
}

//Coloca as posições no ranking
void enumera(score *currentRanking)
{
	int i;
	for (i = 0; i < 100; i++)
	{
		if (currentRanking[i].posicao != 0)
		{
			currentRanking[i].posicao = i;
		}
	}
}

//serve para pegar o ranking atual (chame para pegar o ranking para a tela de ranking)
void getRanking(score *ranking, char nomeArquivo[100])
{
	int i;
	//essa parte serve para o caso de o arquivo estar vazio, seta todos os nomes para "..."
	for (i = 0; i < 100; i++)
	{
		strcpy(ranking[i].nome, "...");
		ranking[i].tempo = 0;
		ranking[i].posicao = -1;
	}
	//aki ele le o ranking atual e salva no array
	ranking_data = fopen(nomeArquivo, "r");
	for (i = 0; i < 100; i++)
	{
		fscanf(ranking_data, "%s%lf%d", ranking[i].nome, &ranking[i].tempo, &ranking[i].posicao);
	}
	fclose(ranking_data);
}

//isso serve basicamente para salvar um rank novo no arquivo (chame depois de uma vitÃ³ria, PS.: lembre de salvar o nome do player, PS2.:o nome so podeter 3 letras se quiser almentar mude na struct)
void changeRankingData(score scoreToAdd)
{
	int i; char archive[100];
	score ranking[100];//array do ranking
	if (dificuldade == 1)
		strcpy(archive, "rankingFACIL.txt");
	else if (dificuldade == 2)
		strcpy(archive, "rankingMEDIO.txt");
	else if (dificuldade == 3)
		strcpy(archive, "rankingDIFICIL.txt");
	getRanking(ranking, archive);//
	addToRanking(scoreToAdd, ranking, 0);//tenta adicionar os dados do scoretoAdd no ranking
	enumera(ranking);
	//escreve os dados do ranking no arquivo
	ranking_data = fopen(archive, "w");
	for (i = 0; i < 100; i++)
	{
		fprintf(ranking_data, "%s %.3lf %d\n", ranking[i].nome, ranking[i].tempo, ranking[i].posicao);
	}
	fclose(ranking_data);
	montaNome(archive);
}
/*Monta os arquivos com os nomes ordenados
pronto para busca binária*/
void montaNome(char archive[100])
{
	score ranking[100];
	getRanking(ranking, archive);
	ordenaNome(ranking);
	if (dificuldade == 1)
		strcpy(archive, "rankingNomeFACIL.txt");
	else if (dificuldade == 2)
		strcpy(archive, "rankingNomeMEDIO.txt");
	else if (dificuldade == 3)
		strcpy(archive, "rankingNomeDIFICIL.txt");
	ranking_nome = fopen(archive, "w");
	int i;
	for (i = 0; i < 100; i ++)
	{
		fprintf(ranking_nome, "%s %.3lf %d\n", ranking[i].nome, ranking[i].tempo, ranking[i].posicao);
	}
	fclose(ranking_nome);
}
//Ordena por nome
void ordenaNome(score *rank)
{
	int o, b;
	score aux;
	for (o = 0; o < 100; o ++)
	{
		for (b = 0; b < 100; b ++)
		{
			if (strcmp(rank[o].nome, rank[b].nome) < 0)
			{
				aux = rank[o];
				rank[o] = rank[b];
				rank[b] = aux;
			}
		}
	}
}
//Busca Binária, procurando pelo nome e retornando a posição no ranking
int buscaBinaria(int inicio, int fim, char nome[4], score *array)
{
	int meio = (inicio + fim) / 2;
	printf("%d %d\n", inicio, fim);
	if (strcmp(array[meio].nome, nome) == 0)
		return(array[meio].posicao);
	else if (fim - inicio <= 1)
		return(-1);
	else if (strcmp(array[meio].nome, nome) < 0)
		buscaBinaria(meio, fim, nome, array);
	else if (strcmp(array[meio].nome, nome) > 0)
		buscaBinaria(inicio, meio, nome, array);
}