// Copyright (c) Benjamín Gajardo (also known as +KZ)

class CLevelHandler
{
    public:
    bool ReadLevel(const char * filename);
    bool SaveLevel(const char * filename);

    bool LoadMenuLevel();
    bool LoadFirstLevel();
    bool LoadLevelNum(unsigned int lvl);
};

extern CLevelHandler g_LevelHandler;
