// Copyright (c) Benjamín Gajardo (also known as +KZ)

class CLevelHandler
{
    public:
    bool ReadLevel(const char * filename);
    bool SaveLevel(const char * filename);
};

extern CLevelHandler g_LevelHandler;
