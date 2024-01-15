#ifndef CONFIG_H
#define CONFIG_H

#define TITLE "Dr. Pellet"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800 

#define RECT_SIZE 30
#define GRID_ROWS 16 // The height of the play-area.
#define GRID_COLUMNS 8 // The width of the play-area.
#define GRID_ROW_CLEARANCE 16 // Viruses will not spawn above or on this ROW.
#define GRID_POSITION_X (WINDOW_WIDTH / 1.5f) - (GRID_ROWS * RECT_SIZE)
#define GRID_POSITION_Y (WINDOW_HEIGHT / 5.5f)

#define PILL_N 128
#define VIRUS_N 66 // The N viruses to be spawned on the grid.

#endif // CONFIG_H