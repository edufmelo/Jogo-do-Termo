#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void arquivo(char[], int, char[]);
void verifica_letracerta(char[], char[]);
void verifica_letraerrada(char[], char[]);
void letras_naoexiste(char[], char[]);
void format_saida(char[], char[]);

int main() {
    char nomeArq[] = "palavras.txt";
    char palavra[6];
    char segredo[6];
    char letra;
    int numero_gerado, i, cont = 0, letra_existe_na_palavra = 0;    
    char letras_certas[26] = {0}; // array para armazenar letras corretas (inicializado com 0) - tamanho 26 para representar as letras do alfabeto
    char letras_erradas[26] = {0}; // array para armazenar letras incorretas (inicializado com 0) - tamanho 26 para representar as letras do alfabeto

    srand(time(NULL));

    numero_gerado = rand() % 401 + 1;

    arquivo(nomeArq, numero_gerado, segredo);
    
	printf("Jogo Termo - Regras:\n");
	printf("- O segredo possui apenas cinco letras;\n");
	printf("- O segredo pode ser previsto em no maximo de 6 TENTATIVAS;\n");
	printf("- *LETRA* = Letra existente no lugar correto;\n");
	printf("- _LETRA_ = Letra existente no lugar incorreto;\n");
	printf("- !LETRA! = Letra nao existente.\n");

    while (cont < 6) {
        printf("\nTentativa %d - Digite uma palavra com cinco letras: ", cont + 1);
        scanf("%s", palavra);

        while (strlen(palavra) != 5) {
            printf("A palavra deve ter exatamente cinco letras!\n");
            printf("\nDigite uma palavra com cinco letras: ");
            scanf("%s", palavra);
        }

        for (i = 0; palavra[i]; i++) {
            palavra[i] = toupper(palavra[i]);
        }

        verifica_letracerta(segredo, palavra);
        verifica_letraerrada(segredo, palavra);
        letras_naoexiste(segredo, palavra);
        printf("\n");
        format_saida(segredo, palavra);

        if (strcmp(segredo, palavra) == 0) { 									// comparação correta de strings (segredo = palavra) - strcmp (compara duas strings)
            printf("\nParabens, voce acertou o segredo! Segredo = %s\n", segredo);
            break; // Encerra o loop se o usuário acertar
        }
		else {
            cont++;
            if (cont < 6) {
                printf("\nTente novamente. Voce tem %d tentativas restantes.\n", 6 - cont);
            } else {
                printf("\nSuas tentativas acabaram. O segredo era: %s\n", segredo);
            }
        }

        // Armazena as letras corretas e incorretas nos conjuntos
        for (i = 0; palavra[i]; i++) {
            letra = palavra[i];
            if (segredo[i] == letra) {
                letras_certas[letra - 'A'] = 1; // marca a letra correta como presente - // (letra - 'A' calcula o índice correto para a letra no array assumindo que as letras são letras maiúsculas consecutivas)
            } else {
            	letras_erradas[letra - 'A'] = 1; // marca a letra errada como presente - // (ou seja, 'A' representa 0, 'B' representa 1, 'C' representa 2, e assim por diante)
            }
        }

        // Exibe as letras corretas
		printf("\nLetras corretas ja digitadas (no lugar certo): ");
		for (i = 0; segredo[i] && palavra[i]; i++) {
		    if (letras_certas[segredo[i] - 'A']) {
		        if (segredo[i] == palavra[i]) {
		            printf("%c ", segredo[i]); 			// marca letras corretas na posição correta 
		        } 
		    }
		}
		printf("\n");

		// Exibe as letras incorretas
		printf("Letras incorretas ja digitadas: ");
		for (i = 0; i < 26; i++) {
		    if (letras_erradas[i]) {
		        letra_existe_na_palavra = 0; // reinicializa a variável para cada letra errada
		        for (int j = 0; segredo[j]; j++) {
		            if (segredo[j] == 'A' + i) {
		                letra_existe_na_palavra = 1;
		                break; // Sai do loop assim que a letra for encontrada na palavra secreta
		            }
		        }
		        if (!letra_existe_na_palavra) {
		            printf("%c ", 'A' + i);
		        }
		    }
		}
		printf("\n");
}

    return 0;
}


void arquivo(char nomeArq[], int numero_gerado, char segredo[]){
    int sorteio_palavras = 0;
    FILE *pf; // ponteiro para file

    pf = fopen (nomeArq, "r");
    if (pf == NULL){
        printf("Algo inesperado aconteceu, pois o arquivo nao foi encontrado!");
        exit(1); // encerrando o programa com ERRO
    }

    while (sorteio_palavras < numero_gerado){
        fgets(segredo, sizeof(segredo), pf);
        sorteio_palavras++;
    }
	segredo[strcspn(segredo, "\n")] = '\0';
    fclose(pf);
}

void verifica_letracerta(char segredo[], char palavra[]) {
    printf("\nLetras no lugar certo: ");
    
    for (int i = 0; segredo[i] && palavra[i]; i++) {
        if (segredo[i] == palavra[i]) {  				// se a letra for igual no segredo e na palavra = letra certa
            printf("%c ", palavra[i]);
        }
    }
    
    printf("\n");
}

void verifica_letraerrada(char segredo[], char palavra[]) {
    printf("Letras no lugar errado: ");
    
    for (int i = 0; segredo[i]; i++) {
        int encontrada = 0;
        
        for (int j = 0; palavra[j]; j++) {
            if (segredo[i] == palavra[j] && segredo[i] != palavra[i]) {
                encontrada = 1;
                break; // Saia do loop interno assim que encontrar uma correspondência no lugar errado.
            }
        }
        if (encontrada) {
            printf("%c ", segredo[i]); // armazenar letras corretas no lugar errado 
        }
    }
    
    printf("\n");
}

void letras_naoexiste(char segredo[], char palavra[]) {
    printf("Letras que nao existem na palavra: ");
    int encontrada = 0;
    
    for (int i = 0; palavra[i]; i++) {
        int existe = 0; // Reinicializa existe para 0 a cada iteração
        
        for (int j = 0; segredo[j]; j++) {
            if (palavra[i] == segredo[j]) {
                existe = 1;
                break; // Sai do loop interno assim que a letra da palavra for encontrada em segredo
            }
        }   
        
        if (!existe) {
            printf("%c ", palavra[i]);
            encontrada = 1;
        }
    }
    
    if (!encontrada) {
        printf("Nenhuma\n");
    }
}

void format_saida(char segredo[], char palavra[]) {
    printf("\nPalavra formatada: ");
    
    for (int i = 0; palavra[i]; i++) {
        char formato = ' ';
        
        if (segredo[i] == palavra[i]) {
            formato = '*';
        } else if (strchr(segredo, palavra[i]) && segredo[i] != palavra[i]) {  //strchr = encontrar a primeira ocorrência de um caractere específico em uma string, ou seja, verifica se a letra da palavra esta em segredo
            formato = '_';
        } else {
            formato = '!';
        }
        
        printf(" %c%c%c ", formato, palavra[i], formato);
    }
    
    printf("\n");
}

