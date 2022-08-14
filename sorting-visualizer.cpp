#include "raylib.h"
#include <vector>
#include <utility>

#define ScreenWidth 1200
#define ScreenHeight 800

#define MinWindowWidth 500
#define MinWindowHeight 800

#define FPS 120

// Column states:
#define NORMAL 0
#define SORTED 1
#define SELECTED 2

// Size of the array
int numberOfColumns = 50;

// Sorting speed
int sortingSpeed = 61;

// Booleans
bool shouldRandomizeArray = true;
bool shouldDisplayMenuView = true;
bool shouldDisplayStartOptions = false;
bool shouldDisplayEndingView = false;
bool shouldStartSorting = false;

bool bubbleSortPressed = false;

bool shouldAddSpeed = false;
bool shouldSubSpeed = false;
bool shouldBeNormalSpeed = false;

bool shouldAddSize = false;
bool shouldSubSize = false;
bool shouldBeNormalSize = false;

// Views
void displayMenuView();
void displayStartOptions();
void displayEndingView();

// Components
void Button(float x, float y, char *Heading, Color color, bool &state);
void BubbleSortButton(float size, char bubbleSortHeading[]);
void StartButton(float size, float fontSize, char startHeading[]);
void RandomizeButton(float size, float fontSize, char randomizeHeading[]);

// Sorting
void sortArray(std::vector<std::pair<int, int>> &arr);
void bubbleSort(std::vector<std::pair<int, int>> &arr);

// Utilities
void drawArray(std::vector<std::pair<int, int>> &arr);
void randomizeArray(std::vector<std::pair<int, int>> &arr);

void changeSize(char operation, int &value);
void changeSpeed(char operation, int &value);

Color findColorForColumn(int columnState);

// The array itself
std::vector<std::pair<int, int>> arr(numberOfColumns);

int main(){

  // Window config:
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithm Visualization");
  SetWindowMinSize(MinWindowWidth, MinWindowHeight);

  // Set FPS:
  SetTargetFPS(FPS);

  while (!WindowShouldClose()){

    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    if (shouldDisplayMenuView)
      displayMenuView();

    shouldDisplayStartOptions = false;
    if (bubbleSortPressed)
      shouldDisplayStartOptions = true;

    if (shouldDisplayStartOptions)
      displayStartOptions();

    if (shouldRandomizeArray)
      randomizeArray(arr);

    if (shouldStartSorting){
      shouldDisplayMenuView = false;
      shouldDisplayStartOptions = false;

      sortArray(arr);

      shouldStartSorting = false;
      shouldDisplayEndingView = true;
    }

    if (shouldDisplayEndingView)
      displayEndingView();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

// VIEWS
void displayMenuView(){
  float fontSize = (2.5 * GetScreenWidth()) / 100;
  char bubbleSortHeading[] = "Bubble Sort";
  float temp = MeasureText(bubbleSortHeading, fontSize) / 2;
  BubbleSortButton(temp, bubbleSortHeading);
  
  drawArray(arr);
};

void displayStartOptions(){
  float fontSize = (2.5 * GetScreenWidth() / 100);
  
  float temp = (27 * GetScreenWidth() / 100);
  char startHeading[] = "Start Sorting";
  StartButton(temp, fontSize, startHeading);

  temp += MeasureText(startHeading, fontSize) + 75;
  char randomizeArrayHeading[] = "Randomize Array";
  RandomizeButton(temp, fontSize, randomizeArrayHeading);

  shouldAddSpeed = false;
  shouldSubSpeed = false;
  shouldBeNormalSpeed = false;

  shouldAddSize = false;
  shouldSubSize = false;
  shouldBeNormalSize = false;

  temp = (84.2 * GetScreenWidth()) / 100;
  char speedButtonText[] = "Speed";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, speedButtonText, BLUE, shouldBeNormalSpeed);

  if (shouldBeNormalSpeed){
    changeSpeed('/', sortingSpeed);
    return;
  }

  temp += MeasureText(speedButtonText, fontSize) + 20;
  char speedPlusButtonText[] = "+";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, speedPlusButtonText, ORANGE, shouldAddSpeed);

  if (shouldAddSpeed){
    changeSpeed('+', sortingSpeed);
    return;
  }

  temp += MeasureText(speedPlusButtonText, fontSize) + 20;
  char speedMinusButtonText[] = "-";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, speedMinusButtonText, ORANGE, shouldSubSpeed);

  if (shouldSubSpeed){
    changeSpeed('-', sortingSpeed);
    return;
  }

  temp = (5 * GetScreenWidth()) / 100;
  char sizeButtonText[] = "Size";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, sizeButtonText, BLUE, shouldBeNormalSize);

  if (shouldBeNormalSize){
    changeSize('/', numberOfColumns);
    return;
  }

  temp += MeasureText(sizeButtonText, fontSize) + 20;
  char sizePlusButton[] = "+";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, sizePlusButton, ORANGE, shouldAddSize);

  if (shouldAddSize){
    changeSize('+', numberOfColumns);
    return;
  }

  temp += MeasureText(sizePlusButton, fontSize) + 20;
  char sizeMinusButton[] = "-";
  Button(temp, GetScreenHeight() / 20 + fontSize * 2, sizeMinusButton, ORANGE, shouldSubSize);

  if (shouldSubSize){
    changeSize('-', numberOfColumns);
    return;
  }
};

void displayEndingView(){
  drawArray(arr);

  shouldDisplayMenuView = true;
  shouldDisplayStartOptions = false;
}

// COMPONENTS
void BubbleSortButton(float size, char bubbleSortHeading[]){
  Color color;
  if (bubbleSortPressed){
    color = BLUE;
  }
  else {
    color = DARKGRAY;
  }

  Button(GetScreenWidth()/2 - size, GetScreenHeight()/20, bubbleSortHeading, color, bubbleSortPressed);
}

void StartButton(float size, float fontSize, char startHeading[]){
  Button(size, GetScreenHeight() / 20 + fontSize * 2, startHeading, DARKGRAY, shouldStartSorting);
  return;
}

void RandomizeButton(float size, float fontSize, char randomizeHeading[]){
  Button(size, GetScreenHeight() / 20 + fontSize * 2, randomizeHeading, DARKGRAY, shouldRandomizeArray);
  return;
}

void Button(float x, float y, char *text, Color color, bool &state){
  float fontSize = (2.5 * GetScreenWidth() / 100);
  Rectangle r1 = {
    .x = x,
    .y = y,
    .width = (float) MeasureText(text, fontSize),
    .height = (float) fontSize,
  };

  if (CheckCollisionPointRec(GetMousePosition(), r1)){
    DrawText(text, x, y, fontSize, DARKGREEN);

    if (IsMouseButtonPressed(0)){
      
      if(state == true)
        state = false;
      else
        state = true;
      return;
    }
  } else {
    DrawText(text, x, y, fontSize, color);
  }
  return;
}

// SORTING

void sortArray(std::vector<std::pair<int, int>> &arr){
  bubbleSort(arr);
  drawArray(arr);
}

void bubbleSort(std::vector<std::pair<int, int>> &arr){
  int endingPoint = numberOfColumns;

  bool swapped;

  do {
    swapped = false;

    for (int i = 0; i < endingPoint - 1; i++){
      arr[i].second = SELECTED;

      if (arr[i] > arr[i+1]){
        std::swap(arr[i], arr[i+1]);
        swapped = true;
      }

      BeginDrawing();
      ClearBackground(LIGHTGRAY);
      
      for (int j = numberOfColumns - 1; j >= endingPoint; j--)
        arr[j].second = SORTED;
      
      drawArray(arr);

      for (int j = i; j >= 0; j--)
        arr[j].second = NORMAL;
      
      EndDrawing();
    }

    endingPoint--;
  } while(swapped);

  for (int i = numberOfColumns - 1; i >= 0; i--)
    arr[i].second = SORTED;
}

// UTILITIES

void drawArray(std::vector<std::pair<int, int>> &arr) {
  float columnWidth = (float) GetScreenWidth() / numberOfColumns;

  for (int i = 0; i < numberOfColumns; i++) {
    Color color = findColorForColumn(arr[i].second);
    
    DrawRectangleV(
      Vector2{(float) i*columnWidth, (float) GetScreenHeight() - arr[i].first},
      Vector2{columnWidth, (float) arr[i].first},
      color
    );
  };
}

void randomizeArray(std::vector<std::pair<int, int>> &arr){
  for (int i = 0; i < numberOfColumns; i++) {
    arr[i] = {GetRandomValue(40, MinWindowWidth - 80), NORMAL};
  }
  shouldRandomizeArray = false;
  return;
}

void changeSize(char operation, int &value){
  switch (operation){
    case '-':
      if (value > 5){
        value -= 5;
        for (int i = 0; i < 5; i++){
          arr.pop_back();
        }
      }
      break;
    case '+':
      value += 5;
      for (int i = 0; i < 5; i++){
        arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL});
      }
      break;
    default:
      while (numberOfColumns > 50){
        numberOfColumns--;
        arr.pop_back();
      }

      while (numberOfColumns < 50){
        numberOfColumns++;
        arr.push_back({GetRandomValue(40, MinWindowWidth-80), NORMAL});
      }
      break;
  }

  for (int i = 0; i < numberOfColumns; i++){
    arr[i].second = NORMAL;
  }

  drawArray(arr);
}

void changeSpeed(char operation, int &value){
  switch (operation){
    case '-':
      value = value - 10 > 0 ? value - 10 : value;
      break;
    case '+':
      value += 10;
      break;
    default:
      value = 61;
      break;
    }
  
  SetTargetFPS(sortingSpeed);
}

Color findColorForColumn(int columnState){
  switch (columnState){
    case SELECTED:
      return BLUE;
      break;
    case SORTED:
      return GREEN;
      break;
    default:
      return RED;
      break;
  }
}
