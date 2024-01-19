#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

struct QueueNode {
    int x, y; 
    struct QueueNode* next; 
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(struct Queue* queue, int x, int y) {
    
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

void dequeue(struct Queue* queue, int* x, int* y) {
    if (queue->front == NULL) {
        *x = -1;
        *y = -1;
        return;
    }

    *x = queue->front->x;
    *y = queue->front->y;

    struct QueueNode* temp = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
}

void placePokeStores(char map[MAP_HEIGHT][MAP_WIDTH], int* exitIndices, int* pokeCenX, int* pokeCenY, int* pokeMartX, int* pokeMartY){
    srand(time(NULL));

    int pcX, pcY;
    do {
        pcX = rand() % (MAP_WIDTH - 3) + 1;  
        pcY = rand() % (MAP_HEIGHT - 3) + 1;
    } while (!(map[pcY][pcX] == ' ' && map[pcY][pcX + 1] ==  ' ' && map[pcY + 1][pcX] == ' ' && map[pcY + 1][pcX + 1] == ' '));

    map[pcY][pcX] = 'C';
    map[pcY][pcX + 1] = 'C';
    map[pcY + 1][pcX] = 'C';
    map[pcY + 1][pcX + 1] = 'C';
    *pokeCenX = pcX;
    *pokeCenY = pcY;
    
    int pmX, pmY; 
    do {
        pmX = rand() % (MAP_WIDTH - 3) + 1; 
        pmY = rand() % (MAP_HEIGHT - 3) + 1;
    } while (!(map[pmY][pmX] == ' ' && map[pmY][pmX + 1] == ' ' && map[pmY + 1][pmX] == ' ' && map[pmY + 1][pmX + 1] == ' '));

    map[pmY][pmX] = 'M';
    map[pmY][pmX + 1] = 'M';
    map[pmY + 1][pmX] = 'M';
    map[pmY + 1][pmX + 1] = 'M';
    *pokeMartX = pmX;
    *pokeMartY = pmY;
}

void findNearestRoad(char map[MAP_HEIGHT][MAP_WIDTH], int startX, int startY, int* nearestX, int* nearestY) {
    struct Queue* queue = createQueue();
    int visited[MAP_HEIGHT][MAP_WIDTH];  
    int dx[] = {-1, 1, 0, 0}; 
    int dy[] = {0, 0, -1, 1}; 

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            visited[i][j] = 0;
        }
    }

    enqueue(queue, startX, startY);
    visited[startY][startX] = 1;  

    while (queue->front != NULL) {
        int x, y;
        dequeue(queue, &x, &y);

        if (map[y][x] == '#') {
            *nearestX = x;
            *nearestY = y;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT && !visited[newY][newX]) {
                enqueue(queue, newX, newY);
                visited[newY][newX] = 1;
            }
        }
    }
    free(queue);
}

void connectPokeStoreToRoad(char map[MAP_HEIGHT][MAP_WIDTH], int pokeStoreX, int pokeStoreY) {
    int nearestRoadX, nearestRoadY;
    findNearestRoad(map, pokeStoreX, pokeStoreY, &nearestRoadX, &nearestRoadY);

    int x = pokeStoreX, y = pokeStoreY;
    while (x != nearestRoadX || y != nearestRoadY) {
        if (x < nearestRoadX) {
            x++;
        } else if (x > nearestRoadX) {
            x--;
        } else if (y < nearestRoadY) {
            y++;
        } else {
            y--;
        }
        if(map[y][x] != 'C' && map[y][x] != 'M'){
            map[y][x] = '#';
        }
    }
}

void placePaths(char map[MAP_HEIGHT][MAP_WIDTH], int* exitIndices) {

    srand(time(NULL));

    
    int nsPathStart = exitIndices[0];   
    int ewPathStart = exitIndices[1];   

    int i, j;
    for (i = 1; i < MAP_HEIGHT; i++) {
        nsPathStart = (nsPathStart < 2) ? 2 : nsPathStart;  
        nsPathStart = (nsPathStart >= MAP_WIDTH - 2) ? MAP_WIDTH - 3 : nsPathStart;  

        int moveDirection = rand() % 3 - 1; 
        map[i][nsPathStart] = '#';   
        if(moveDirection != 0){
            nsPathStart += moveDirection;
            if (map[i][nsPathStart] != 'C' && map[i][nsPathStart] != 'M' && map[i][nsPathStart] != '%'){
                map[i][nsPathStart] = '#';
            }
        }

    }

    for (j = 1; j < MAP_WIDTH; j++) {
        
        ewPathStart = (ewPathStart < 2) ? 2 : ewPathStart; 
        ewPathStart = (ewPathStart >= MAP_HEIGHT - 2) ? MAP_HEIGHT - 3 : ewPathStart;

        int moveDirection = rand() % 3 - 1;
        map[ewPathStart][j] = '#';
        if (moveDirection != 0) {
            ewPathStart += moveDirection;
            if(map[ewPathStart][j] != 'C' && map[ewPathStart][j] != 'M' && map[ewPathStart][j] != '%'){
                map[ewPathStart][j] = '#';
            }
            
        }
    }
}

void generateTerrain(char map[MAP_HEIGHT][MAP_WIDTH], int* exitIndices) {

    srand(time(NULL));
    
    exitIndices[0] = rand() % (MAP_WIDTH - 4);
    exitIndices[1] = rand() % (MAP_HEIGHT - 4);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = ' ';
        }
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
        map[0][x] = '%';
        map[MAP_HEIGHT - 1][x] = '%';
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        map[y][0] = '%';
        map[y][MAP_WIDTH - 1] = '%';
    }

    map[0][exitIndices[0]] = '#';
    map[exitIndices[1]][0] = '#';

    for (int i = 0; i < 2; i++) {
        int grassRegionX = rand() % (MAP_WIDTH - 10) + 5;
        int grassRegionY = rand() % (MAP_HEIGHT - 10) + 5;

        for (int y = grassRegionY; y < grassRegionY + 15; y++) {
            for (int x = grassRegionX; x < grassRegionX + 15; x++) {
                if (map[y][x] == ' ') {
                    map[y][x] = ':';
                }
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        int grassRegionX = rand() % (MAP_WIDTH - 10) + 5;
        int grassRegionY = rand() % (MAP_HEIGHT - 10) + 5;

        for (int y = grassRegionY; y < grassRegionY + 12; y++) {
            for (int x = grassRegionX; x < grassRegionX + 12; x++) {
                if (map[y][x] == ' ') {
                    map[y][x] = '.';
                }
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        int grassRegionX = rand() % (MAP_WIDTH - 10) + 5;
        int grassRegionY = rand() % (MAP_HEIGHT - 10) + 5;

        for (int y = grassRegionY; y < grassRegionY + 6; y++) {
            for (int x = grassRegionX; x < grassRegionX + 6; x++) {
                if (map[y][x] == ' ') {
                    map[y][x] = '^';
                }
            }
        }
    }

    int waterRegionX = rand() % (MAP_WIDTH - 12) + 6;
    int waterRegionY = rand() % (MAP_HEIGHT - 12) + 6;

    for (int y = waterRegionY; y < waterRegionY + 25; y++) {
        for (int x = waterRegionX; x < waterRegionX + 25; x++) {
            if (map[y][x] == ' ') {
                map[y][x] = '~';
            }
        }
    }
}


void fill(char map[MAP_HEIGHT][MAP_WIDTH]){
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
            if(map[y][x] == ' '){
                map[y][x] = '.';
            }
        }
    }

    srand(time(NULL));
    int count = 0; 
    while (count < 10) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;
        if (map[y][x] == '.') {
            if (rand() % 2 == 0) {
                map[y][x] = '%';
            } else {
                map[y][x] = '^';
            }
            count++;
        }
    }
}

void printMapWithColors(char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = map[y][x];
            switch (cell) {
                case '%':
                    printf("\033[31m"); // Red color for '%'
                    break;
                case '~':
                    printf("\033[36m"); // Blue color for '~'\033[36m
                    break;
                case '.':
                    printf("\033[32m"); // Green color for '.'
                    break;
                case ':':
                    printf("\033[35m"); // Magenta color for ':'
                    break;
                case '^':
                    printf("\033[34m"); // Cyan color for '^'
                    break;
                case '#':
                    printf("\033[33m"); // Yellow color for '#'
                    break;
                default:
                    printf("\033[0m");  // Reset to default color
                    break;
            }
            printf("%c", cell);
        }
        printf("\033[0m\n");
    }
}


int main() {
    char map[MAP_HEIGHT][MAP_WIDTH];
    int exitIndices[2];
    int pokemonCenX, pokemonCenY, pokeMartX, pokeMartY; 
    generateTerrain(map, exitIndices);
    placePaths(map, exitIndices);
    placePokeStores(map, exitIndices, &pokemonCenX, &pokemonCenY, &pokeMartX, &pokeMartY);
    connectPokeStoreToRoad(map, pokemonCenX, pokemonCenY);  
    connectPokeStoreToRoad(map, pokeMartX, pokeMartY);
    fill(map);
    printMapWithColors(map);

    return 0;
}
