#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* COP 3502C Assignment 1
This program is written by: James Phillips */

// 6/2/24
// This is a program to take a list of ingredients, smoothie recipes with
// relative portions, and then take lists of smoothie orders and corresponding
// weights for multiple stores and display how much of each ingredient each store
// needs in total to supply their full smoothie demand.

typedef struct item {
int itemNo;
int portions;
} item;

typedef struct recipe {
int itemCount;
item* itemList;
int totalPortions;
} recipe;

// Pre-condition: reference to a variable to store number of ingredients.
// Post-condition: Reads in numIngredients and that number of strings from
// the inputs, allocates an array of
// strings to store the input, and sizes each
// individual string dynamically to be the
// proper size (string length plus 1), and
// returns a pointer to the array.
char** readIngredients(int *numIngredients){
  // Allocate memory for array of strings of size numIngredients
  // Also declare temporary variable later used for string memory allocation
  scanf("%d", numIngredients);
  char** names = malloc(sizeof(char*) * *numIngredients);
  char temp[21];

  // Allocate size of temp string for each ingredient and then copy the sting over to names
  for(int i = 0; i < *numIngredients; i++){
    scanf("%s", temp);
    names[i] = malloc((sizeof(char) + 1) * strlen(temp));
    strcpy(names[i], temp);
  }

  return names;
}

// Pre-condition: does not take any parameter
// Post-condition: Reads in details of a recipe such as numItems,
// Dynamically allocates space for a single
// recipe, dynamically allocates an array of
// item of the proper size, updates the
// numItems field of the struct, fills the
// array of items appropriately based on the
// input and returns a pointer to the struct
// dynamically allocated.
recipe* readRecipe(){
  // Initialize recipe variables and memory
  recipe* singleRecipe = malloc(sizeof(recipe));
  scanf("%d", &singleRecipe->itemCount);
  singleRecipe->itemList = malloc(sizeof(item) * singleRecipe->itemCount);
  singleRecipe->totalPortions = 0;

  // Get info for each item and increment total portions by each item portion
  for(int i = 0; i < singleRecipe->itemCount; i++){
    scanf("%d %d", &singleRecipe->itemList[i].itemNo, &singleRecipe->itemList[i].portions);
    singleRecipe->totalPortions += singleRecipe->itemList[i].portions;
  }

  return singleRecipe;
}

// Pre-condition: reference to a variable to store number of recipes.
// Post-condition: Read number of recipes. Dynamically allocates an array of 
// pointers to recipes of size numRecipes, reads numRecipes
// number of recipes from standard input, creates
// structs to store each recipe and has the
// pointers point to each struct, in the order
// the information was read in. (Should call
// readRecipe in a loop.)
  recipe** readAllRecipes(int *numRecipes){
    // Allocate memory for size numRecipes
    scanf("%d", numRecipes);
    recipe** recipes = malloc(sizeof(recipe*) * *numRecipes);

    // Call readRecipe for each recipe in numRecipes
    for(int i = 0; i < *numRecipes; i++){
      recipes[i] = readRecipe();
    }

    return recipes;
  }

// Pre-condition: 0 < numSmoothies <= 100000, recipeList is
// pointing to the list of all smoothie recipes and
// numIngredients equals the number of total ingredients (you have 
// already read it in the first line of the input).
// Post-condition: Reads in information from input file
// about numSmoothies number of smoothie orders and dynamically
// allocates an array of doubles of size numIngredients such
// that index i stores the # of pounds of ingredient i
// needed to fulfill all smoothie orders and returns a pointer
// to the array.
double* calculateOrder(int ingredientCount, int numSmoothies, recipe** recipeList){
  // Initialize variables and allocate memory for ingredient list
  // Use calloc so the ingredients are initialized to zero so I can increment
  // the total weight needed as I loop through the smoothies
  // Could also use malloc and then loop through and initialize them as zero
  int recipeIndex, weight;
  double* ingredients = calloc(sizeof(double), ingredientCount);
  scanf("%d", &numSmoothies);

  // For each smoothie, get input for smoothie number and weight
  for(int i = 0; i < numSmoothies; i++){
    scanf("%d %d", &recipeIndex, &weight);

    // For each ingredient in each smoothie, increment the corresponding ingredient
    // in the ingredient list by (portion * weight) / totalPortions. 
    // * 1.0 is added in the numerator to prevent integer division
    for(int j = 0; j < recipeList[recipeIndex]->itemCount; j++){
      ingredients[recipeList[recipeIndex]->itemList[j].itemNo] += (recipeList[recipeIndex]->itemList[j].portions * weight * 1.0) / recipeList[recipeIndex]->totalPortions;
    }
  }

  return ingredients;
}

// Pre-conditions: ingredientNames store the names of each
// ingredient and orderList stores the amount
// to order for each ingredient, and both arrays
// are of size numIngredients.
// Post-condition: Prints out a list, in ingredient order, of each
// ingredient, a space and the amount of that
// ingredient to order rounded to 6 decimal
// places. One ingredient per line.
void printOrder(char** ingredientNames, double* orderList, int numIngredients){
  // For each ingredient, print the name and corresponding weight if the
  // weight isn't zero
  for(int i = 0; i < numIngredients; i++){
    if(orderList[i] != 0){
      printf("%s %.6f\n", ingredientNames[i], orderList[i]);
    }
  }
  printf("\n");
}

// Pre-conditions: ingredientList is an array of char* of size
// numIngredients with each char* dynamically allocated.
// Post-condition: all the memory pointed to by ingredientList is
// freed.
void freeIngredients(char** ingredientList, int numIngredients){
  // For each ingredient in ingredient list, free the char array
  // Finally free the array of char arrays
  for(int i = 0; i < numIngredients; i++){
    free(ingredientList[i]);
  }
  free(ingredientList);
}


// Pre-conditions: allRecipes is an array of recipe* of size
// numRecipes with each recipe* dynamically allocated
// to point to a single recipe.
// Post-condition: all the memory pointed to by allRecipes is
// freed.
void freeRecipes(recipe** allRecipes, int numRecipes){
  for(int i = 0; i < numRecipes; i++){
    free(allRecipes[i]->itemList);
    free(allRecipes[i]);
  }
  free(allRecipes);
}

int main(void) {
  int numIngredients, numRecipes, numStores;

  // Get list of ingredient names and recipes
  char** names = readIngredients(&numIngredients);
  recipe** recipes = readAllRecipes(&numRecipes);

  // For each store, calculate the total weight of ingredients and print
  // using the printOrder function. Also free the ingredient weights list afterwards.
  scanf("%d", &numStores);
  for(int i = 0; i < numStores; i++){
    double* ingredients = calculateOrder(numIngredients, 0, recipes);

    printf("List of items for store %d:\n", i+1);
    printOrder(names, ingredients, numIngredients);
    free(ingredients);
  }

  // Calling the free functions
  freeIngredients(names, numIngredients);
  freeRecipes(recipes, numRecipes);
}