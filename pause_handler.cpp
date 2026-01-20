// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "pause_handler.h"
#include <config_handler.h>
#include <input_handler.h>
#include <level_handler.h>
#include <helper_ui.h>
#include <globals.h>
#include <cstdio>
#include <cstring>
#include <game.h>

#define MAX_PAGES 5

CPauseHandler g_PauseHandler;

//each page must always end with nullptr
static const char * s_PagePaused[] = {
    "Resume",
    "Retry",
    "Settings",
    "Main menu",
    nullptr,
};

static const char * s_PageMainMenu[] = {
    "Continue",
    "New Game",
    "Settings",
    "Exit",
    nullptr,
};

static const char * s_PageSettings[] = {
    "Keyboard",
    "Mouse",
    "", // fullscreen
    "Go Back",
    nullptr,
};

static const char * s_PageKeyboard[] = {
    "",
    "Go Back",
    nullptr,
};

static const char * s_PageMouse[] = {
    "",
    "",
    "Go Back",
    nullptr,
};

static const char ** s_OptionsList[MAX_PAGES] = {
    s_PagePaused, // 0
    s_PageMainMenu, // 1
    s_PageSettings, // 2
    s_PageKeyboard, // 3
    s_PageMouse, // 4
};

void CPauseHandler::HandleMenuOption(unsigned int Page, int Option)
{
    if(Option < 0)
        return;

    if(Page == 0 || Page == 1)
    {
        if(Option == 2)
        {
            m_CurrentPage = 2;
            return;
        }
    }
    if(Page == 0)
    {
        if(Option == 0) //Resume
        {
            m_IsPaused = false;
            return;
        }
        if(Option == 1) //Retry
        {
            g_LevelHandler.LoadLevelNum(g_ConfigHandler.m_GameProgress.m_CurrentLevelNumber);
            m_IsPaused = false;
            return;
        }
        if(Option == 3) //Main Menu
        {
            g_ConfigHandler.SaveGameProgress();
            g_LevelHandler.LoadMenuLevel();
            return;
        }
    }
    if(Page == 1)
    {
        if(Option == 1) //New game
        {
            g_LevelHandler.LoadFirstLevel();
            return;
        }
        if(Option == 3) //Exit
        {
            g_Game.m_Exit = true;
            return;
        }
    }
    if(Page == 2)
    {
        if(Option == 0)
        {
            m_CurrentPage = 3;
            return;
        }
        if(Option == 1)
        {
            m_CurrentPage = 4;
            return;
        }
        if(Option == 2)
        {
            ToggleBorderlessWindowed();
            g_ConfigHandler.m_Config.m_Fullscreen ^= 1;
            if(g_ConfigHandler.m_Config.m_Fullscreen)
            {
                SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor()));
                SetWindowPosition(0,0);
            }
            else
            {
                SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
                SetWindowPosition(GetMonitorWidth(GetCurrentMonitor()) / 2 - GAME_WIDTH/2, GetMonitorHeight(GetCurrentMonitor()) / 2 - GAME_HEIGHT/2);
            }
            g_Globals.m_CurrentWindowWidth = GetScreenWidth();
            g_Globals.m_CurrentWindowHeight = GetScreenHeight();
            return;
        }
        if(Option == 3)
        {
            g_ConfigHandler.SaveConfig();
            m_CurrentPage = m_IsMenu ? 1 : 0;
            return;
        }
    }
    if(Page == 3)
    {
        if(Option == 1)
        {
            g_ConfigHandler.SaveConfig();
            m_CurrentPage = 2;
            return;
        }
    }
    if(Page == 4)
    {
        if(Option == 2)
        {
            g_ConfigHandler.SaveConfig();
            m_CurrentPage = 2;
            return;
        }
    }
}

const char *CPauseHandler::GetOptionTextLabel(unsigned int Page, int Option)
{
    if(Page == 2)
    {
        if(Option == 2)
        {
            if(IsWindowFullscreen() || (g_Globals.m_CurrentWindowWidth == GetMonitorWidth(GetCurrentMonitor()) && g_Globals.m_CurrentWindowHeight == GetMonitorHeight(GetCurrentMonitor())))
            {
                return "Fullscreen";
            }
            else
            {
                return "Windowed";
            }
        }
    }

    return nullptr;
}

void CPauseHandler::Update()
{
    if(m_IsMenu)
        m_IsPaused = false;

    const SInput * pInput = g_InputHandler.GetInputs();

    static bool waiting_for_release_esc = false;

    if(!m_IsMenu)
    {
        if(!m_IsPaused)
        {
            if(pInput->m_Escape)
            {
                if(!waiting_for_release_esc)
                {
                    m_IsPaused = true;
                    waiting_for_release_esc = true;
                }
            }
            else
            {
                waiting_for_release_esc = false;
            }
        }
        else
        {
            if(pInput->m_Escape)
            {
                if(!waiting_for_release_esc)
                {
                    m_IsPaused = false;
                    waiting_for_release_esc = true;
                }
            }
            else
            {
                waiting_for_release_esc = false;
            }
        }
    }

    if(m_IsPaused || m_IsMenu)
    {
        if(IsCursorHidden())
        {
            ShowCursor();
            SetMousePosition(g_Globals.m_CurrentWindowWidth/2, g_Globals.m_CurrentWindowHeight/8);
            g_InputHandler.UpdateMousePos();
        }

        if(m_IsMenu && m_CurrentPage == 0)
            m_CurrentPage = 1;
        else if(m_IsPaused && m_CurrentPage == 1)
            m_CurrentPage = 0;

        static bool waiting_for_key_release = false;

        int option = 0;
        m_Selected = false;
        UIButtonState state = BTNSTATE_NONE;
        while(s_OptionsList[m_CurrentPage][option])
        {
            const char * pText = GetOptionTextLabel(m_CurrentPage, option);
            if(!pText)
                pText = s_OptionsList[m_CurrentPage][option];
            
            state = UpdateUIButtonCenterScaledToScreen(pInput, g_Globals.m_MainFont, pText, g_Globals.m_CurrentWindowWidth/2,
                g_Globals.m_CurrentWindowHeight/3 + ((option + 1.5) * g_Globals.m_CurrentWindowHeight/8), 5);
            
            if(state == BTNSTATE_HOVER || state == BTNSTATE_PRESSED)
                break;

            option++;
        }

        int num_options = 0;
        while(s_OptionsList[m_CurrentPage][num_options])
        {
            num_options++;
        }

        if(state == BTNSTATE_PRESSED)
        {
            if(!waiting_for_key_release)
            {
                m_HoverOption = option;
                m_Selected = true;
                waiting_for_key_release = true;
            }
        }
        else if(state == BTNSTATE_NONE || state == BTNSTATE_HOVER)
        {
            if(state == BTNSTATE_HOVER)
                m_HoverOption = option;

            //try with keyboard controls
            if(pInput->m_MouseClick) //Mouse click seems bugged on window resize
            {
                waiting_for_key_release = true;
            }
            else if(pInput->m_ArrowUp)
            {
                if(!waiting_for_key_release)
                {
                    m_HoverOption--;
                    if(m_HoverOption < 0)
                        m_HoverOption = num_options-1;
                    waiting_for_key_release = true;
                }
            }
            else if(pInput->m_ArrowDown)
            {
                if(!waiting_for_key_release)
                {
                    m_HoverOption++;
                    if(m_HoverOption >= num_options)
                        m_HoverOption = 0;
                    waiting_for_key_release = true;
                }
            }
            else if(pInput->m_Enter)
            {
                if(!waiting_for_key_release)
                {
                    m_Selected = true;
                    waiting_for_key_release = true;
                }
            }
            else
            {
                waiting_for_key_release = false;
            }
        }

        if(m_Selected && m_HoverOption >= 0 && m_HoverOption < num_options)
        {
            HandleMenuOption(m_CurrentPage, m_HoverOption);
        }
    }
    else
    {
        if(!g_Game.m_EditorMode)
        {
            if(!IsCursorHidden())
            {
                HideCursor();
            }
        }
    }
}

void CPauseHandler::Render()
{
    if(m_IsPaused || m_IsMenu)
    {
        const SInput * pInput = g_InputHandler.GetInputs();

        if(m_IsPaused)
        {
            DrawTextCenterScaledToScreen(g_Globals.m_MainFont, "Paused", g_Globals.m_CurrentWindowWidth/2, g_Globals.m_CurrentWindowHeight/8, 5);
        }

        int option = 0;
        while(s_OptionsList[m_CurrentPage][option])
        {
            const char * pText = GetOptionTextLabel(m_CurrentPage, option);
            if(!pText)
                pText = s_OptionsList[m_CurrentPage][option];
            RenderUIButtonCenterScaledToScreen(pInput, g_Globals.m_MainFont, pText, g_Globals.m_CurrentWindowWidth/2,
                g_Globals.m_CurrentWindowHeight/3 + ((option + 1.5) * g_Globals.m_CurrentWindowHeight/8), 5, m_HoverOption == option ? GRAY : RAYWHITE);
            option++;
        }
    }
}
