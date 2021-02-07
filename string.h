struct Concatenation;
typedef struct Concatenation Concatenation;

struct String;
typedef struct String String;

struct Rope;
typedef struct Rope Rope;
Rope* Rope_reference(Rope*);

struct Concatenation {
  Rope* s0;
  Rope* s1;
};

void Concatenation_initialize(Concatenation* self, Rope* s0, Rope* s1) {
  self->s0 = Rope_reference(s0);
  self->s1 = Rope_reference(s1);
}

Concatenation Concatenation_create(Rope* s0, Rope* s1) {
  Concatenation result;
  Concatenation_initialize(&result, s0, s1);
  return result;
}

struct String {
  size_t length;
  char* characters;
};

void String_initialize(String* self, size_t length, char* characters) {
  self->length = length;
  self->characters = characters;
}

void String_initializeFromCString(String* self, char* cString) {
  size_t length = strlen(cString);
  char* characters = calloc(length, sizeof(char));

  memset(characters, 0, length);
  memcpy(characters, cString, length);

  String_initialize(self, length, characters);
}

String String_create(size_t length, char* characters) {
  String result;
  String_initialize(&result, length, characters);
  return result;
}

String String_createFromCString(char* characters) {
  String result;
  String_initializeFromCString(&result, characters);
  return result;
}

struct Rope {
  union {
    Concatenation concatenation;
    String string;
  } content;

  size_t referenceCount;
  bool isConcatenation;
};

void Rope_initializeFromCString(Rope* self, char* cString) {
  self->content.string = String_createFromCString(cString);
  self->referenceCount = 0;
  self->isConcatenation = false;
}

void Rope_initialize_from_concatenation(Rope* self, Rope* s0, Rope* s1) {
  self->content.concatenation = Concatenation_create(s0, s1);
  self->referenceCount = 0;
  self->isConcatenation = true;
}

Rope* Rope_constructFromCString(char* cString) {
  Rope* result = calloc(1, sizeof(Rope));
  Rope_initializeFromCString(result, cString);
  return result;
}

Rope* Rope_concatenate(Rope* self, Rope* s0, Rope* s1) {
  Rope* result = calloc(1, sizeof(Rope));
  Rope_initialize_from_concatenation(result, s0, s1);
  return result;
}

void Rope_deinitialize(Rope* self) {
  // If this fails, we're calling Rope_deinitialize on a string that is still
  // referenced
  assert(self->referenceCount == 0);

  if(self->isConcatenation) {
    Rope_deinitialize(self->content.concatenation.s0);
    Rope_deinitialize(self->content.concatenation.s1);
  } else {
    free(self->content.string.characters);
  }
}

void Rope_destruct(Rope* self) {
  // If this fails, we're calling Rope_destruct without a matching
  // Rope_reference
  assert(self->referenceCount > 0);

  // TODO Make this thread safe
  self->referenceCount--;

  if(self->referenceCount > 0) return;

  Rope_deinitialize(self);
  free(self);
}

Rope* Rope_reference(Rope* self) {
  // TODO Make this thread safe
  self->referenceCount++;
  return self;
}

void Rope_print(Rope* self) {
  if(self->isConcatenation) {
    Rope_print(self->content.concatenation.s0);
    Rope_print(self->content.concatenation.s1);
  }
  else {
    fwrite(
        self->content.string.characters,
        sizeof(char),
        self->content.string.length,
        stdout
    );
    fflush(stdout);
  }
}
