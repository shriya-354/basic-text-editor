/*Copyright 2016 Shriya Shende
This program is a part of the project Charlie.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Save_h
#define Save_h

int getLastLine(WINDOW *editWin,int YMax,int XMax);
int getLastChar(WINDOW *editWin,int XMax);
char* save(WINDOW *editWin,FILE *fp,int lastLine);
int isPrint(WINDOW *editWin,WINDOW *comWin,FILE *fp,int *prev,int ch,int *flag,int lastLine);

#endif
