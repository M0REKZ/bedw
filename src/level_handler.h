/*
    Brain Evil: Dark World
    Copyright (C) Benjamín Gajardo (also known as +KZ)

    This program is shared under the PLUSKAIZO LICENSE, you should have received
    a copy of the license along with the program.
*/
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
