#pragma once


typedef struct {
    int x;
    int y;
    int xr;
    int yr;
} laser_node;


void laser_node_init(laser_node *n);

void do_fell_x(laser_node *n, int x, int midiChannel);
void do_fell_y(laser_node *n, int y, int midiChannel);

void do_read_x(laser_node *n, int x, int midiChannel);
void do_read_y(laser_node *n, int y, int midiChannel);

void midiPosOn(int x, int y, int channel);
void midiPosOff(int x, int y, int channel);
int posToNote(int x, int y);
