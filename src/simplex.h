/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef SIMPLEX_H_INCLUDED
#define SIMPLEX_H_INCLUDED

float noise(float xin, float yin, float zin);
float simplex_noise(int octaves, float x, float y, float z);

#endif // SIMPLEX_H_INCLUDED
