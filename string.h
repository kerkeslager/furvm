struct StringConcatenation;
typedef struct StringConcatenation StringConcatenation;

struct String;
typedef struct String String;
String* String_reference(String*);

struct StringConcatenation {
  String* s0;
  String* s1;
};

void StringConcatenation_initialize(StringConcatenation* self, String* s0, String* s1) {
  self->s0 = String_reference(s0);
  self->s1 = String_reference(s1);
}

StringConcatenation StringConcatenation_create(String* s0, String* s1) {
  StringConcatenation result;
  StringConcatenation_initialize(&result, s0, s1);
  return result;
}

struct String {
  union {
    char* simpleString;
    StringConcatenation concatenation;
  } rope;

  size_t referenceCount;
  bool isConcatenation;
};

void String_initialize(String* self, char* simpleString) {
  self->rope.simpleString = simpleString;
  self->referenceCount = 0;
  self->isConcatenation = false;
}

void String_initialize_from_concatenation(String* self, String* s0, String* s1) {
  self->rope.concatenation = StringConcatenation_create(s0, s1);
  self->referenceCount = 0;
  self->isConcatenation = true;
}

String* String_construct_from_c_string(char* simpleString) {
  String* result = malloc(sizeof(String));
  String_initialize(result, simpleString);
  return result;
}

String* String_concatenate(String* self, String* s0, String* s1) {
  String* result = malloc(sizeof(String));
  String_initialize_from_concatenation(result, s0, s1);
  return result;
}

void String_deinitialize(String* self) {
  // If this fails, we're calling String_deinitialize on a string that is still
  // referenced
  assert(self->referenceCount == 0);

  if(self->isConcatenation) {
  } else {
  }
}

void String_destruct(String* self) {
  // If this fails, we're calling String_destruct without a matching
  // String_reference
  assert(self->referenceCount > 0);

  // TODO Make this thread safe
  self->referenceCount--;

  if(self->referenceCount > 0) return;

  String_deinitialize(self);
  free(self);
}

String* String_reference(String* self) {
  // TODO Make this thread safe
  self->referenceCount++;
  return self;
}

void String_print(String* self) {
  if(self->isConcatenation) {
    String_print(self->rope.concatenation.s0);
    String_print(self->rope.concatenation.s1);
  }
  else {
    printf("%s", self->rope.simpleString);
  }
}
