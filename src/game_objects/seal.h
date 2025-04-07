#ifndef SEAL_H
#define SEAL_H

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <SDL_image.h>
#include "../global.h"

struct Point {
    double x, y;
};

class Seal {
    public:
        Seal(int _x, int _y);
        ~Seal();

        int getX() { return x; }
        int getY() { return y; }

        void setX(int _x) { x = _x; }
        void setY(int _y) { y = _y; }
        void setPos(int _x, int _y) { x = _x; y = _y; }

        void drawLine(int _x, int _y) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 211, 0.5);
            SDL_RenderDrawLine(renderer, x, y, _x, _y);
            SDL_SetRenderDrawColor(renderer, BG_R, BG_G, BG_B, 1);
        }

        static bool includes(int x, int y, std::vector<Seal*> *seals) {
            int num_vertices = seals->size();
            bool inside = false;
        
            Point p1 = { x: seals->at(0)->getX() * 1.0, y: seals->at(0)->getY() * 1.0 }, p2;
        
            for (int i = 1; i <= num_vertices; i++) {
                Seal* seal = seals->at(i % num_vertices);
                p2.x = seal->getX() * 1.0;
                p2.y = seal->getY() * 1.0;
        
                if (y > std::min(p1.y, p2.y)) {
                    if (y <= std::max(p1.y, p2.y)) {
                        if (x <= std::max(p1.x, p2.x)) {
                            double x_intersection
                                = (y - p1.y) * (p2.x - p1.x)
                                    / (p2.y - p1.y)
                                + p1.x;
                                
                            if (p1.x == p2.x
                                || x <= x_intersection) {
                                inside = !inside;
                            }
                        }
                    }
                }

                p1 = p2;
            }
            printf("Point (%d, %d) is inside: %s\n", x, y, (inside) ? "True" : "False");
            return inside;
        }

    private:
        int x, y = 0;
};

#endif