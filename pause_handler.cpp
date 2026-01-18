// Copyright (c) Benjamín Gajardo (also known as +KZ)

#include "pause_handler.h"
#include <input_handler.h>
#include <level_handler.h>
#include <helper_ui.h>
#include <globals.h>
#include <cstdio>
#include <cstring>
#include <game.h>

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

static const char ** s_OptionsList[5] = {
    s_PagePaused, // 0
    s_PageMainMenu, // 1
};

void CPauseHandler::HandleMenuOption(const char * pOption)
{
    if(!pOption)
        return;

    printf("%s\n",pOption);

    if(!strncmp(pOption, "Resume", sizeof("Resume")))
    {
        m_IsPaused = false;
        return;
    }
    
    if(!strncmp(pOption, "New Game", sizeof("New Game")))
    {
        g_LevelHandler.LoadFirstLevel();
        return;
    }

    if(!strncmp(pOption, "Main menu", sizeof("Main menu")))
    {
        g_LevelHandler.LoadMenuLevel();
        return;
    }

    if(!strncmp(pOption, "Exit", sizeof("Exit")))
    {
        g_Game.m_Exit = true;
        return;
    }
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
            state = UpdateUIButtonCenterScaledToScreen(pInput, g_Globals.m_MainFont, s_OptionsList[m_CurrentPage][option], g_Globals.m_CurrentWindowWidth/2,
                g_Globals.m_CurrentWindowHeight/3 + ((option + 1) * g_Globals.m_CurrentWindowHeight/8), 5);
            
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

            if(pInput->m_ArrowUp)
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
            HandleMenuOption(s_OptionsList[m_CurrentPage][m_HoverOption]);
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
            RenderUIButtonCenterScaledToScreen(pInput, g_Globals.m_MainFont, s_OptionsList[m_CurrentPage][option], g_Globals.m_CurrentWindowWidth/2,
                g_Globals.m_CurrentWindowHeight/3 + ((option + 1) * g_Globals.m_CurrentWindowHeight/8), 5, m_HoverOption == option ? GRAY : RAYWHITE);
            option++;
        }
    }
}
