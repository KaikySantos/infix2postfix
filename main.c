#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 200

void push(char stack [], char elem, int *top) {
	if (*top == (MAX - 1)) {
		printf("Error\n");
  } else {
		stack[*top += 1] = elem;		
  }
}

void pop(int *top) {
	if (*top == -1) {
		printf("Error\n");
  } else {
		*top -= 1;
  }
}

void reverse(char expression[]) {
	int i, j;
	char tmp;
	
	i = (strlen(expression) - 1);
	j = 0;

	while(i > j) {
		tmp = expression[i];
		expression[i] = expression[j];
		expression[j] = tmp;
		i--;
		j++;
	}
}

void addSpace(char expression[], int *j) {
	if(expression[*j - 1] != ' ') {
		expression[*j] = ' ';
		*j += 1;
	}
}

int operator(char el) {
	int val = 0;
	
	if ((el == '+') || (el == '-') || (el == '*') || (el == '/')) {
		val = 1;
	}

	return val;
}

int getExpression(char infixExpression []) {
	int i, j, parenthesis, notValidExpression;
	char *tempExpression, notReadChar;
	size_t lenght_s;

	tempExpression = (char *)calloc(MAX, sizeof(char));
	
	if (fgets(tempExpression, MAX, stdin));
	
	lenght_s = strlen(tempExpression);

	if (tempExpression[lenght_s - 1] == '\n') {
		tempExpression[lenght_s - 1] = 0;	
  } else {
		printf("Expression is too long. \n");
		while ((notReadChar != '\n') && (notReadChar != EOF)) {
			notReadChar = getchar();
		}
	}
	
	parenthesis = 0;
	i = 0;
	j = 0;
	notValidExpression = 0;	

	do {
		if (((isdigit(tempExpression[i])) || (tempExpression[i] == '(') || (tempExpression[i] == ')') || (operator(tempExpression[i]) == 1))) {
			if ((i > 0) && (operator(tempExpression[i]) == 1) && (operator(tempExpression[i - 1]) == 1)) {
				notValidExpression = 1;
			} else if ((operator(tempExpression[i]) == 1) && (tempExpression[i + 1] == ')')) {
				notValidExpression = 1;
			} else if((tempExpression[i] == '(') && (operator(tempExpression[i + 1]) == 1)) {
				notValidExpression = 1;
			} else {
				infixExpression[j] = tempExpression[i];
				j++;
			}
		}
		
		if (tempExpression[i] == '(') {
			parenthesis++;
			if ((i != 0) && (tempExpression[i - 1] == ')'))
				notValidExpression = 1;				
		} else if (tempExpression[i] == ')') {
			parenthesis--;
			if ((i != 0) && (tempExpression[i - 1] == '(')) {
				notValidExpression = 1;
			}
		}

		if (parenthesis < 0) {
			notValidExpression = 1;
		}

		i++;
	} while ((tempExpression[i] != '\0') && (notValidExpression != 1));

	if(parenthesis != 0) {
		notValidExpression = 1;
	}

	if ((operator(infixExpression[0]) == 1)) {
		notValidExpression = 1;
	}

	if (operator(infixExpression[strlen(infixExpression) - 1]) == 1) {
		notValidExpression = 1;
	}

	if (infixExpression[0] == '\0') {
		notValidExpression = 1;
	}
	
	free(tempExpression);
	
	return notValidExpression;
}

int emptyStack(int top) {
	int val = 0;

	if (top == -1) {
		val = 1;
  }

  return val;
}

int getPrecedence(char el) {
	int prec = -1;
	
	switch(el) {
		case ')':
			prec = 0;
			break;
		case '+':
			prec = 1;
			break;
		case '-':
			prec = 1;
			break;
		case '*':
			prec = 2;
			break;
		case '/':
			prec = 2;
			break;
	}

	return prec;
}

int precedence(char top, char elem) {
	return getPrecedence(elem) <= getPrecedence(top) ? 1:0;
}

void infixToPostfix(char stack[], char infix[], char postfix[]) {
	int i, top, j;
	char elem_temp;
	
	j = 0;
	top = 0;

	for (i = 0; infix[i] != '\0'; i++) {
		elem_temp = infix[i];
		
		if (isdigit(elem_temp)) {
			postfix[j] = elem_temp;
			j++;

			if((operator(infix[i + 1]) == 1) || ((infix[i + 1] == ')') && (operator(infix[i + 2]) == 1))) {
        addSpace(postfix, &j);
      }
		} else if (elem_temp == '(') {
			push(stack, elem_temp, &top);
		} else if (elem_temp == ')') {
			while ((emptyStack(top) == 0) && (stack[top] != '(')) {
				addSpace(postfix, &j);
				postfix[j] = stack[top];
				j++;
				pop(&top);
				addSpace(postfix, &j);
			}

			pop(&top);
		} else {
			while ((emptyStack(top) == 0) && (precedence(stack[top],elem_temp) == 1)) {
				postfix[j] = stack[top];
				j++;
				pop(&top);
				addSpace(postfix, &j);
			}

			push(stack, elem_temp, &top);
		}
	}

	while (emptyStack(top) == 0) {
		addSpace(postfix, &j);
		postfix[j] = stack[top];
		j++;
		pop(&top);
	}

	if(postfix[strlen(postfix) - 1] == ' ') {
		postfix[strlen(postfix) - 1] = '\0';
  } else {
		postfix[j] = '\0';
  }
}

int main(void) {
	char *infix, *output, *stack;
	int notValidExpression;
	
	infix = (char *)calloc(MAX, sizeof(char));
	output = (char *)calloc(MAX * 2, sizeof(char));
	stack = (char *)calloc(MAX, sizeof(char));

  notValidExpression = 0;

  printf("Infix: ");
  
  notValidExpression = getExpression(infix);
  
  if (notValidExpression == 0) {
    infixToPostfix(stack, infix, output);
    printf("\nPostfix: %s \n", output);
  } else {
    printf("Expression is not valid.");
  }
  
  memset(infix, '\0', MAX);
  memset(output, '\0', MAX);
  memset(stack, '\0', MAX);				
	
	return(0);
}
