typedef struct FnPrototype FnPrototype;
typedef struct Environment Environment;

struct FnPrototype {
  int arity;
  char* name;
  char* ret;
  char** args;
};

struct Environment {
  int fnc;
  FnPrototype** fns;
};

