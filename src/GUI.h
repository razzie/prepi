/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

class Globals;
class Level;

class GUI
{
public:
    GUI(Globals*, Level* = nullptr);
    ~GUI();
    void setLevel(Level*);
    void update();

private:
    float getHealthRate();
    float getRewardRate();
    void drawHealthBar();
    void drawRewardBar();

    Globals* m_globals;
    Level* m_level;
};

#endif // GUI_H_INCLUDED
