typedef struct FnPrototype FnPrototype_t;
typedef struct Environment Environment_t;

struct FnPrototype {
  int arity;
  char* name;
  char* ret;
  char** args;
};

struct Environment {
  int tmpc;
  int fnc;
  FnPrototype_t** fns;
};
