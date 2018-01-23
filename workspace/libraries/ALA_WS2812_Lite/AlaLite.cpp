/*--------------------------------------------------------------------
  This file is part of the Arduino ALA library.

  The Arduino ALA library is free software: you can redistribute it
  and/or modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  The Arduino ALA library is distributed in the hope that it will be
  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with The Arduino ALA library.  If not, see
  <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/

#include "AlaLite.h"

int getStep(long t0, long t, int v)
{
  return ((millis()-t0)%t)*v/t;
}

float getStepFloat(long t0, long t, float v)
{
  return ((millis()-t0)%t)*v/t;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
