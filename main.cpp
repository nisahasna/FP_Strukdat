#include "raylib.h"
#include <vector>
#include <cmath>
#include <string>

static float DistSqr(Vector2 a, Vector2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

class Ball {
public:
    Vector2 pos;
    Vector2 vel;
    float r;

    Ball(float x, float y) {
        pos = {x, y};
        vel = {(float)GetRandomValue(-150,150)/100.0f,
               (float)GetRandomValue(-150,150)/100.0f};
        r = 12.0f; // ukuran medium
    }

    void Update(float dt) {
        pos.x += vel.x * dt * 200;
        pos.y += vel.y * dt * 200;

        if (pos.x < r) { pos.x = r; vel.x *= -1; }
        if (pos.x > GetScreenWidth()-r) { pos.x = GetScreenWidth()-r; vel.x *= -1; }
        if (pos.y < r) { pos.y = r; vel.y *= -1; }
        if (pos.y > GetScreenHeight()-r) { pos.y = GetScreenHeight()-r; vel.y *= -1; }
    }

    void Draw() const {
        DrawCircleV(pos, r, BLUE);
    }
};

//QUADTREE
struct AABB {
    float x, y, w, h;

    bool Contains(const Ball* b) const {
        return (b->pos.x >= x && b->pos.x < x+w &&
                b->pos.y >= y && b->pos.y < y+h);
    }

    bool Intersects(const AABB& other) const {
        return !(other.x > x+w || other.x+other.w < x ||
                 other.y > y+h || other.y+other.h < y);
    }
};

class Quadtree {
public:  //NODE QUADTREE
    static const int CAPACITY = 4;
    AABB boundary;
    std::vector<Ball*> points;
    bool divided;

    Quadtree *nw, *ne, *sw, *se;

    Quadtree(AABB b) : boundary(b), divided(false),
        nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr) {}

    ~Quadtree() {
        delete nw; delete ne; delete sw; delete se;
    }

    void Subdivide() {
        float x = boundary.x;
        float y = boundary.y;
        float w = boundary.w/2;
        float h = boundary.h/2;

        nw = new Quadtree({x, y, w, h});
        ne = new Quadtree({x+w, y, w, h});
        sw = new Quadtree({x, y+h, w, h});
        se = new Quadtree({x+w, y+h, w, h});
        divided = true;
    }

    bool Insert(Ball* b) {
        if (!boundary.Contains(b)) return false;

        if (points.size() < CAPACITY && !divided) {
            points.push_back(b);
            return true;
        }

        if (!divided) Subdivide();

        if (nw->Insert(b)) return true;
        if (ne->Insert(b)) return true;
        if (sw->Insert(b)) return true;
        if (se->Insert(b)) return true;

        return false;
    }

    void Query(const AABB& range, std::vector<Ball*>& found) {
        if (!boundary.Intersects(range)) return;

        for (Ball* b : points) {
            if (range.Contains(b)) found.push_back(b);
        }

        if (divided) {
            nw->Query(range, found);
            ne->Query(range, found);
            sw->Query(range, found);
            se->Query(range, found);
        }
    }

    void Draw() const {
        DrawRectangleLines(boundary.x, boundary.y, boundary.w, boundary.h, GRAY);
        if (divided) {
            nw->Draw(); ne->Draw(); sw->Draw(); se->Draw();
        }
    }
};

// Collision response
void ResolveCollision(Ball& a, Ball& b) {
    Vector2 diff = {a.pos.x - b.pos.x, a.pos.y - b.pos.y};
    float dist = sqrtf(diff.x*diff.x + diff.y*diff.y);
    float minDist = a.r + b.r;

    if (dist == 0 || dist >= minDist) return;

    // Normalisasi
    diff.x /= dist;
    diff.y /= dist;

    // Pisahkan posisi (anti nempel)
    float overlap = 0.5f * (minDist - dist);
    a.pos.x += diff.x * overlap;
    a.pos.y += diff.y * overlap;
    b.pos.x -= diff.x * overlap;
    b.pos.y -= diff.y * overlap;

    // Tukar kecepatan (sederhana tapi stabil)
    std::swap(a.vel, b.vel);
}

//BRUTEFORCE
void CollideBruteforce(std::vector<Ball>& balls) {
    for (size_t i = 0; i < balls.size(); i++) {
        for (size_t j = i+1; j < balls.size(); j++) {
            ResolveCollision(balls[i], balls[j]);
        }
    }
}

void CollideQuadtree(std::vector<Ball>& balls) {
    Quadtree qt({0,0,(float)GetScreenWidth(),(float)GetScreenHeight()});

    for (auto& b : balls) qt.Insert(&b);

    for (auto& b : balls) {
        AABB range = {b.pos.x-30, b.pos.y-30, 60, 60};
        std::vector<Ball*> found;
        qt.Query(range, found);

        for (Ball* other : found) {
            if (&b >= other) continue; // hindari double check
            ResolveCollision(b, *other);
        }
    }

    //qt.Draw();
}

int main() {
    InitWindow(1000, 700, "FP_Strukdat");
    SetTargetFPS(60);

    const int BALL_COUNT = 100;
    std::vector<Ball> balls;
    for (int i = 0; i < BALL_COUNT; i++) {
        balls.emplace_back(GetRandomValue(50,950), GetRandomValue(50,650));
    }

    bool useQuadtree = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            useQuadtree = !useQuadtree;
        }

        for (auto& b : balls) b.Update(dt);

        if (useQuadtree)
            CollideQuadtree(balls);
        else
            CollideBruteforce(balls);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto& b : balls) b.Draw();

        DrawText(useQuadtree ? "Mode: QUADTREE (SPACE)"
                             : "Mode: BRUTEFORCE (SPACE)",
                 20, 20, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
