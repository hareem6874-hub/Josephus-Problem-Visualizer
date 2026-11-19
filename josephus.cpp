#include <SFML/Graphics.hpp>
#include <algorithm>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<string>
#include<iostream>
using namespace std;

template<typename T>
class Queue
{
private:
    T* arr;
    int capacity;
    int front;
    int rear;
    int size;
public:
    Queue()  //Default Constructor
    {
        capacity = 100;
        front = 0;
        rear = 0;
        size = 0;

        arr = new T[capacity];
    }
    bool isEmpty()
    {
        return size == 0;
    }
    bool isFull()
    {
        return size == capacity;
    }
    int getSize()
    {
        return size;
    }
    void enqueue(T value)
    {
        if (isFull()) //if queue is full, increase its capacity
        {
            resize(capacity * 2);
        }
        arr[rear] = value;
        rear = (rear + 1) % capacity;
        size++;
    }
    T dequeue() //Removes and returns the front element
    {
        if (size == 0)

        {
            return T();
        }
        T value = arr[front];
        front = (front + 1) % capacity; //moves front circularly
        size--;
        return value;
    }
    T getFront()
    {
        if (size == 0)
        {
            return T();
        }
        return arr[front];
    }
    T getAt(int i)  //O(1) read of the i-th element from the front (used only to DRAW the queue)
    {
        if (i < 0 || i >= size)
        {
            return T();
        }
        return arr[(front + i) % capacity];
    }
    void resize(int newCapacity)
    {
        if (newCapacity < size)
        {
            newCapacity = size;
        }

        T* newArr = new T[newCapacity];
        for (int i = 0; i < size; i++) //Copy queue elements in their order
        {
            newArr[i] = arr[(front + i) % capacity];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
        front = 0;
        rear = size;
    }
    void clear()
    {
        front = 0;
        rear = 0;
        size = 0;
    }

    ~Queue()
    {
        delete[]arr;
    }
};

// =====================================================================
//                    GUI THEME + HELPERS (drawing only)
// =====================================================================
const float PI_VAL = 3.14159265f;

const sf::Color cBgTop(10, 11, 15);
const sf::Color cBgBottom(19, 21, 29);
const sf::Color cHeader(14, 16, 21);
const sf::Color cPanel(20, 22, 29);
const sf::Color cBorder(42, 46, 58);
const sf::Color cText(233, 235, 240);
const sf::Color cDim(126, 133, 150);
const sf::Color cAccent(214, 178, 108);     // champagne gold
const sf::Color cAccentHi(238, 208, 150);
const sf::Color cDanger(214, 98, 108);
const sf::Color cSuccess(72, 187, 140);
const sf::Color cAmber(226, 146, 68);
const sf::Color cNodeFill(26, 30, 40);
const sf::Color cNodeBorder(84, 92, 114);
const sf::Color cDeadFill(17, 19, 25);
const sf::Color cDeadBorder(52, 56, 68);

sf::Color lerpColor(sf::Color a, sf::Color b, float t)
{
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return sf::Color((sf::Uint8)(a.r + (b.r - a.r) * t),
        (sf::Uint8)(a.g + (b.g - a.g) * t),
        (sf::Uint8)(a.b + (b.b - a.b) * t),
        (sf::Uint8)(a.a + (b.a - a.a) * t));
}

float textWidth(sf::Font& f, const string& s, unsigned size, bool bold)
{
    sf::Text t;
    t.setFont(f);
    t.setString(s);
    t.setCharacterSize(size);
    if (bold)
    {
        t.setStyle(sf::Text::Bold);
    }
    sf::FloatRect b = t.getLocalBounds();
    return b.left + b.width;
}

// 1 -> "1st", 2 -> "2nd", 3 -> "3rd", 4 -> "4th" ...
string ordinal(int v)
{
    int m100 = v % 100;
    int m10 = v % 10;
    string suffix = "th";
    if (m100 < 11 || m100 > 13)
    {
        if (m10 == 1) suffix = "st";
        else if (m10 == 2) suffix = "nd";
        else if (m10 == 3) suffix = "rd";
    }
    return to_string(v) + suffix;
}

// Draws text. align: 0 = left, 1 = center, 2 = right (vertically centered on y)
void drawText(sf::RenderWindow& w, sf::Font& f, const string& s, unsigned size,
    sf::Color c, float x, float y, int align, bool bold = false)
{
    sf::Text t;
    t.setFont(f);
    t.setString(s);
    t.setCharacterSize(size);
    t.setFillColor(c);
    if (bold)
    {
        t.setStyle(sf::Text::Bold);
    }
    sf::FloatRect b = t.getLocalBounds();
    float ox = b.left;
    if (align == 1)
    {
        ox = b.left + b.width / 2;
    }
    else if (align == 2)
    {
        ox = b.left + b.width;
    }
    t.setOrigin(ox, b.top + b.height / 2);
    t.setPosition(x, y);
    w.draw(t);
}

// Flat rectangle (no border)
void drawRect(sf::RenderWindow& w, float x, float y, float wd, float h, sf::Color fill)
{
    sf::RectangleShape r(sf::Vector2f(wd, h));
    r.setPosition(x, y);
    r.setFillColor(fill);
    w.draw(r);
}

// Rounded rectangle
void drawRound(sf::RenderWindow& w, float x, float y, float wd, float h, float r, sf::Color c)
{
    sf::RectangleShape a(sf::Vector2f(wd - 2 * r, h));
    a.setPosition(x + r, y);
    a.setFillColor(c);
    w.draw(a);

    sf::RectangleShape b(sf::Vector2f(wd, h - 2 * r));
    b.setPosition(x, y + r);
    b.setFillColor(c);
    w.draw(b);

    sf::CircleShape corner(r);
    corner.setPointCount(24);
    corner.setFillColor(c);
    corner.setPosition(x, y);
    w.draw(corner);
    corner.setPosition(x + wd - 2 * r, y);
    w.draw(corner);
    corner.setPosition(x, y + h - 2 * r);
    w.draw(corner);
    corner.setPosition(x + wd - 2 * r, y + h - 2 * r);
    w.draw(corner);
}

// Rounded box with a border of thickness bt
void drawBox(sf::RenderWindow& w, float x, float y, float wd, float h, float r,
    sf::Color fill, sf::Color border, float bt)
{
    drawRound(w, x, y, wd, h, r, border);
    drawRound(w, x + bt, y + bt, wd - 2 * bt, h - 2 * bt, r - bt, fill);
}

void drawLine(sf::RenderWindow& w, float x1, float y1, float x2, float y2,
    float th, sf::Color c)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float len = sqrt(dx * dx + dy * dy);
    sf::RectangleShape r(sf::Vector2f(len, th));
    r.setOrigin(0, th / 2);
    r.setPosition(x1, y1);
    r.setRotation(atan2(dy, dx) * 180 / PI_VAL);
    r.setFillColor(c);
    w.draw(r);
}

// Soft radial glow (fades from `alpha` at the center to 0 at the edge)
void drawGlow(sf::RenderWindow& w, float cx, float cy, float r, sf::Color c, int alpha)
{
    const int seg = 48;
    sf::VertexArray va(sf::TriangleFan, seg + 2);
    va[0] = sf::Vertex(sf::Vector2f(cx, cy), sf::Color(c.r, c.g, c.b, (sf::Uint8)alpha));
    for (int i = 0; i <= seg; i++)
    {
        float a = 2 * PI_VAL * i / seg;
        va[i + 1] = sf::Vertex(sf::Vector2f(cx + cos(a) * r, cy + sin(a) * r), sf::Color(c.r, c.g, c.b, 0));
    }
    w.draw(va);
}

// Thin circular progress arc (starts at the top, goes clockwise)
void drawArc(sf::RenderWindow& w, float cx, float cy, float rIn, float rOut, float frac, sf::Color c)
{
    if (frac <= 0)
    {
        return;
    }
    if (frac > 1)
    {
        frac = 1;
    }
    int seg = (int)(100 * frac) + 1;
    sf::VertexArray va(sf::TriangleStrip, (seg + 1) * 2);
    for (int i = 0; i <= seg; i++)
    {
        float a = -PI_VAL / 2 + frac * 2 * PI_VAL * i / seg;
        va[2 * i] = sf::Vertex(sf::Vector2f(cx + cos(a) * rIn, cy + sin(a) * rIn), c);
        va[2 * i + 1] = sf::Vertex(sf::Vector2f(cx + cos(a) * rOut, cy + sin(a) * rOut), c);
    }
    w.draw(va);
}

// Arc along a circle from angle a0 spanning `span` radians; fades in from the tail to the head
void drawSweep(sf::RenderWindow& w, float cx, float cy, float rIn, float rOut, float a0, float span, sf::Color c)
{
    if (span <= 0)
    {
        return;
    }
    int seg = (int)(span * 40) + 2;
    sf::VertexArray va(sf::TriangleStrip, (seg + 1) * 2);
    for (int i = 0; i <= seg; i++)
    {
        float t = (float)i / seg;
        float a = a0 + span * t;
        sf::Color vc(c.r, c.g, c.b, (sf::Uint8)(c.a * t));
        va[2 * i] = sf::Vertex(sf::Vector2f(cx + cos(a) * rIn, cy + sin(a) * rIn), vc);
        va[2 * i + 1] = sf::Vertex(sf::Vector2f(cx + cos(a) * rOut, cy + sin(a) * rOut), vc);
    }
    w.draw(va);
}

// Card: soft shadow, hairline border, top highlight, optional gold-marked title
void drawCard(sf::RenderWindow& w, sf::Font& f, float x, float y, float wd, float h, const string& title)
{
    for (int i = 0; i < 6; i++)
    {
        drawRound(w, x - i, y - i + 5, wd + 2 * i, h + 2 * i, 10 + i, sf::Color(0, 0, 0, 20));
    }
    drawBox(w, x, y, wd, h, 10, cPanel, cBorder, 1);
    drawRect(w, x + 12, y + 1, wd - 24, 1, sf::Color(255, 255, 255, 14));
    if (title != "")
    {
        drawRect(w, x + 16, y + 13, 3, 10, cAccent);
        drawText(w, f, title, 10, cDim, x + 26, y + 18, 0, true);
    }
}

// Button: primary = gold, secondary = outlined graphite
void drawButton(sf::RenderWindow& w, sf::Font& f, sf::FloatRect rect, const string& label,
    bool primary, bool hover, bool pressed)
{
    sf::Color fill = primary ? cAccent : sf::Color(25, 28, 37);
    sf::Color border = primary ? cAccent : sf::Color(62, 68, 86);
    sf::Color txt = primary ? sf::Color(24, 18, 6) : cText;
    if (pressed)
    {
        fill = primary ? sf::Color(150, 122, 70) : sf::Color(16, 18, 24);
        border = primary ? fill : sf::Color(50, 55, 70);
    }
    else if (hover)
    {
        fill = primary ? cAccentHi : sf::Color(34, 38, 50);
        border = primary ? cAccentHi : sf::Color(120, 128, 150);
    }
    drawBox(w, rect.left, rect.top, rect.width, rect.height, 8, fill, border, 1);
    drawText(w, f, label, 13, txt, rect.left + rect.width / 2, rect.top + rect.height / 2 - 1, 1, true);
}

int main()
{
    Queue<int>q;
    int n;
    int k;
    int s;
    srand(time(0));   //Initializing random number generator
    cout << "Enter N: ";
    cin >> n;
    if (n <= 1)
    {
        cout << "Invalid input!" << endl;
        return 0;
    }
    cout << "Enter K: ";
    cin >> k;
    if (k <= 0)
    {
        cout << "Invalid input!" << endl;
        return 0;
    }
    s = rand() % n + 1; //Selecting random starting point between 1 and N

    cout << "Starting Point: " << s << endl;

    for (int i = 1; i <= n; i++)
    {
        q.enqueue(i);
    }

    while (q.getFront() != s)  //MAking starting person comes at front
    {
        int person = q.dequeue();
        q.enqueue(person);
    }

    const int W = 1100;
    const int H = 740;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;   //smooth edges
    sf::RenderWindow window(   //Create sfml window
        sf::VideoMode(W, H),
        "Josephus Problem",
        sf::Style::Titlebar | sf::Style::Close,
        settings
    );
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"))
        {
            if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
            {
                return 1;
            }
        }
    }

    // ---------------- ARENA GEOMETRY ----------------
    float centerX = 380;
    float centerY = 348;
    float radius = 200;
    bool  started = false;
    sf::Clock clock;       //controlling time bw executions
    sf::Clock animClock;   //only for visual fading
    sf::Clock frameClock;  //frame time for smooth pointer movement

    int lastExecuted = 0;
    int winner = 0;
    int currentPerson = s;

    string statusString = "Ready";
    int pointTarget = 0;      //(display only) person the arrow aims at right after an execution
    float pointTime = -10;

    float personRadius = (2 * 3.14f * radius / n) * 0.35f;  //sizes of persons
    if (personRadius > 28)
    {
        personRadius = 28;
    }
    if (personRadius < 4)
    {
        personRadius = 4;
    }

    bool showLabels = (personRadius >= 9);   //very small circles (large N) get a callout tag instead

    // arrays for circles and their labels
    sf::CircleShape* people = new sf::CircleShape[n];
    sf::Text* labels = new sf::Text[n];

    for (int i = 0; i < n; i++)  //creating people
    {
        float angle = -PI_VAL / 2 + i * 2 * PI_VAL / n;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        people[i].setRadius(personRadius);
        people[i].setPointCount(50);
        people[i].setPosition(x - personRadius, y - personRadius);

        labels[i].setFont(font);
        labels[i].setString(to_string(i + 1));
        if (n <= 10)
        {
            labels[i].setCharacterSize(18);
        }
        else if (n <= 25)
        {
            labels[i].setCharacterSize(14);
        }
        else
        {
            labels[i].setCharacterSize(11);
        }
        sf::FloatRect labelBounds = labels[i].getLocalBounds();
        labels[i].setOrigin
        (
            labelBounds.left + labelBounds.width / 2, labelBounds.top + labelBounds.height / 2
        );
        labels[i].setPosition(x, y);
    }
    bool* alive = new bool[n]; //track of still aliv people
    for (int i = 0; i < n; i++)
    {
        alive[i] = true;
    }

    // --- display-only data ---
    int* order = new int[n];           //elimination order (for the log)
    int orderCount = 0;
    float* deathTime = new float[n];   //when each person died (for the fade)
    float* flashTime = new float[n];   //when each person was last counted/skipped
    int* countNo = new int[n];         //the count number (1..K) they were given in the last step
    for (int i = 0; i < n; i++)
    {
        deathTime[i] = -10;
        flashTime[i] = -10;
        countNo[i] = 0;
    }
    float ptrAng = -PI_VAL / 2 + (s - 1) * 2 * PI_VAL / n;   //smoothly animated pointer angle
    float shownFrac = 0;   //smoothly animated progress value
    float* hi = new float[n];          //eased "current person" highlight (0..1) per person
    for (int i = 0; i < n; i++)
    {
        hi[i] = 0;
    }
    long long enqOps = 0;              //(display only) enqueue operations made by the simulation steps
    long long deqOps = 0;              //(display only) dequeue operations made by the simulation steps
    float sweepFrom = 0;               //(display only) counting sweep along the ring
    float sweepSpan = 0;
    float sweepTime = -10;
    float winnerTime = -10;
    float toastTime = -10;

    // background gradient
    sf::VertexArray bg(sf::TriangleStrip, 4);
    bg[0] = sf::Vertex(sf::Vector2f(0, 0), cBgTop);
    bg[1] = sf::Vertex(sf::Vector2f(W, 0), cBgTop);
    bg[2] = sf::Vertex(sf::Vector2f(0, H), cBgBottom);
    bg[3] = sf::Vertex(sf::Vector2f(W, H), cBgBottom);

    // button areas
    sf::FloatRect startRect(760, 636, 100, 44);
    sf::FloatRect pauseRect(870, 636, 100, 44);
    sf::FloatRect resetRect(980, 636, 100, 44);

    auto doStart = [&]()
        {
            if (winner == 0)
            {
                started = true;
                clock.restart();
                cout << "START Button Clicked!\n";
            }
        };

    auto doReset = [&]()
        {
            started = false;
            lastExecuted = 0;
            winner = 0;
            orderCount = 0;
            currentPerson = s;
            pointTarget = 0;
            enqOps = 0;
            deqOps = 0;
            sweepTime = -10;
            winnerTime = -10;
            statusString = "Ready";
            q.clear();  //empty the queue
            for (int i = 1; i <= n; i++)  //adding all people back into queue
            {
                q.enqueue(i);
            }
            while (q.getFront() != s) //move queue back to starting point
            {
                int person = q.dequeue();
                q.enqueue(person);
            }
            for (int i = 0; i < n; i++) //marking everyone alive again
            {
                alive[i] = true;
                deathTime[i] = -10;
                flashTime[i] = -10;
                hi[i] = 0;
            }
            ptrAng = -PI_VAL / 2 + (s - 1) * 2 * PI_VAL / n;
            clock.restart(); //restart timer
            cout << "RESET Button Clicked!\n";
        };

    while (window.isOpen())
    {
        float now = animClock.getElapsedTime().asSeconds();
        float dt = frameClock.restart().asSeconds();
        if (dt > 0.05f)
        {
            dt = 0.05f;
        }
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    if (started)
                    {
                        started = false;
                    }
                    else
                    {
                        doStart();
                    }
                }
                if (event.key.code == sf::Keyboard::R)
                {
                    doReset();
                }
                if (event.key.code == sf::Keyboard::S)   //save a PNG snapshot of the window
                {
                    sf::Texture snap;
                    snap.create(window.getSize().x, window.getSize().y);
                    snap.update(window);
                    snap.copyToImage().saveToFile("josephus_snapshot.png");
                    toastTime = now;
                    cout << "Snapshot saved\n";
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (startRect.contains(mousePos))
                    {
                        doStart();
                    }
                    if (pauseRect.contains(mousePos))
                    {
                        started = false;
                    }
                    if (resetRect.contains(mousePos))
                    {
                        doReset();
                    }
                }
            }
        }
        if (started && clock.getElapsedTime().asSeconds() >= 1.0f)  //execute oner personper second
        {
            clock.restart();
            if (q.getSize() > 1)  //while more than one person still alive
            {
                int startP = q.getFront();   //(display only) where this step's counting begins
                for (int i = 1; i < k; i++) //skip k-1 people
                {
                    int person = q.dequeue();
                    q.enqueue(person);
                    enqOps++;
                    deqOps++;
                    flashTime[person - 1] = now;   //(display only) mark the counted person
                    countNo[person - 1] = i;
                }
                int executed = q.dequeue();  // the kth person is executed
                deqOps++;
                cout << "Executed: " << executed << endl;
                alive[executed - 1] = false;  //marking this person as dead
                deathTime[executed - 1] = now;
                flashTime[executed - 1] = now;   //(display only)
                countNo[executed - 1] = k;
                sweepFrom = -PI_VAL / 2 + (startP - 1) * 2 * PI_VAL / n;   //(display only) arc start -> executed
                sweepSpan = (-PI_VAL / 2 + (executed - 1) * 2 * PI_VAL / n) - sweepFrom;
                while (sweepSpan < 0)
                {
                    sweepSpan += 2 * PI_VAL;
                }
                sweepTime = now;
                order[orderCount++] = executed;
                lastExecuted = executed;
                pointTarget = executed;   //arrow first points at the executed person...
                pointTime = now;
                currentPerson = q.getFront(); //next person becomes current
                statusString = "Executed " + to_string(executed) + "   |   Next " + to_string(currentPerson);
            }
            else
            {
                winner = q.getFront();
                winnerTime = now;
                cout << "Winner: " << winner << endl;
                statusString = "Winner: person " + to_string(winner);
                started = false;
            }
        }

        // ---------- smooth pointer movement ----------
        int aim = currentPerson;   //...then, after a moment, swings to the next person
        if (pointTarget != 0 && now - pointTime < 0.6f)
        {
            aim = pointTarget;
        }
        float targetAng = -PI_VAL / 2 + (aim - 1) * 2 * PI_VAL / n;
        float diff = targetAng - ptrAng;
        while (diff > PI_VAL)
        {
            diff -= 2 * PI_VAL;
        }
        while (diff < -PI_VAL)
        {
            diff += 2 * PI_VAL;
        }
        ptrAng += diff * min(1.0f, dt * 8.0f);

        // ====================== DRAWING ======================
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.clear(cBgTop);
        window.draw(bg);

        // ---- header ----
        drawRect(window, 0, 0, W, 60, cHeader);
        drawRect(window, 0, 60, W, 1, cBorder);
        drawRect(window, 0, 60, 260, 1, cAccent);   //gold underline accent
        {
            sf::CircleShape logo(10);          //small ring logo
            logo.setPointCount(40);
            logo.setOrigin(10, 10);
            logo.setPosition(38, 30);
            logo.setFillColor(sf::Color::Transparent);
            logo.setOutlineThickness(2);
            logo.setOutlineColor(cAccent);
            window.draw(logo);
            sf::CircleShape dot(3.5f);
            dot.setPointCount(20);
            dot.setOrigin(3.5f, 3.5f);
            dot.setPosition(38, 20);
            dot.setFillColor(cAccentHi);
            window.draw(dot);
        }
        drawText(window, font, "Josephus", 22, cText, 62, 29, 0, true);
        float tw = textWidth(font, "Josephus", 22, true);
        drawText(window, font, "Problem", 22, cDim, 62 + tw + 8, 29, 0);
        drawText(window, font, "Circular elimination  /  Queue simulation", 12, cDim, W - 28, 30, 2);

        // ---- arena ----
        drawCard(window, font, 20, 76, 720, 544, "");

        drawGlow(window, centerX, centerY, radius + 40, cAccent, 22);   //soft warm center light

        sf::CircleShape ring(radius + personRadius + 24);
        ring.setPointCount(140);
        ring.setOrigin(ring.getRadius(), ring.getRadius());
        ring.setPosition(centerX, centerY);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(1);
        ring.setOutlineColor(sf::Color(40, 45, 58));
        window.draw(ring);

        sf::CircleShape track(radius);
        track.setPointCount(140);
        track.setOrigin(radius, radius);
        track.setPosition(centerX, centerY);
        track.setFillColor(sf::Color::Transparent);
        track.setOutlineThickness(1);
        track.setOutlineColor(sf::Color(50, 56, 72));
        window.draw(track);

        sf::CircleShape inner(radius * 0.5f);
        inner.setPointCount(120);
        inner.setOrigin(radius * 0.5f, radius * 0.5f);
        inner.setPosition(centerX, centerY);
        inner.setFillColor(sf::Color::Transparent);
        inner.setOutlineThickness(1);
        inner.setOutlineColor(sf::Color(32, 36, 47));
        window.draw(inner);

        // dial ticks (one per position)
        for (int i = 0; i < n; i++)
        {
            float a = -PI_VAL / 2 + i * 2 * PI_VAL / n;
            float r1 = radius + personRadius + 11;
            bool isCur = (i + 1 == currentPerson);
            float r2 = r1 + (isCur ? 9 : 5);
            sf::Color tc = isCur ? cAccent : (alive[i] ? sf::Color(74, 82, 102) : sf::Color(42, 46, 58));
            drawLine(window, centerX + cos(a) * r1, centerY + sin(a) * r1,
                centerX + cos(a) * r2, centerY + sin(a) * r2, isCur ? 2.f : 1.f, tc);
        }

        // legend (top-left corner of arena)
        {
            sf::CircleShape m(5);
            m.setPointCount(30);
            m.setOrigin(5, 5);
            m.setOutlineThickness(1);

            m.setPosition(42, 102);
            m.setFillColor(cNodeFill);
            m.setOutlineColor(cNodeBorder);
            window.draw(m);
            drawText(window, font, "Alive", 12, cDim, 56, 101, 0);

            m.setPosition(42, 122);
            m.setFillColor(cAccent);
            m.setOutlineColor(cAccent);
            window.draw(m);
            drawText(window, font, "Current", 12, cDim, 56, 121, 0);

            m.setPosition(42, 142);
            m.setFillColor(cDeadFill);
            m.setOutlineColor(cDanger);
            window.draw(m);
            drawText(window, font, "Executed", 12, cDim, 56, 141, 0);

            m.setPosition(42, 162);
            m.setFillColor(cSuccess);
            m.setOutlineColor(cSuccess);
            window.draw(m);
            drawText(window, font, "Winner", 12, cDim, 56, 161, 0);
        }

        // rule caption (top-right of the arena)
        drawText(window, font, "Every " + ordinal(k) + " person is eliminated", 12, cDim, 722, 102, 2);
        drawText(window, font, "Counting begins at person " + to_string(s), 12, cDim, 722, 122, 2);

        // pointer from the center to the current person (fades in from the hub)
        {
            float dx = cos(ptrAng);
            float dy = sin(ptrAng);
            float px = -dy;
            float py = dx;
            float sx = centerX + dx * 56;
            float sy = centerY + dy * 56;
            float ex = centerX + dx * (radius - personRadius - 14);
            float ey = centerY + dy * (radius - personRadius - 14);
            sf::Color pc = (winner != 0) ? cSuccess : cAccent;
            sf::VertexArray beam(sf::TriangleStrip, 4);
            beam[0] = sf::Vertex(sf::Vector2f(sx + px * 1.2f, sy + py * 1.2f), sf::Color(pc.r, pc.g, pc.b, 0));
            beam[1] = sf::Vertex(sf::Vector2f(sx - px * 1.2f, sy - py * 1.2f), sf::Color(pc.r, pc.g, pc.b, 0));
            beam[2] = sf::Vertex(sf::Vector2f(ex + px * 1.2f, ey + py * 1.2f), sf::Color(pc.r, pc.g, pc.b, 235));
            beam[3] = sf::Vertex(sf::Vector2f(ex - px * 1.2f, ey - py * 1.2f), sf::Color(pc.r, pc.g, pc.b, 235));
            window.draw(beam);
            sf::CircleShape tip(5, 4);   //small diamond marker
            tip.setOrigin(5, 5);
            tip.setPosition(ex, ey);
            tip.setFillColor(pc);
            window.draw(tip);
        }

        // counting sweep: a gold arc runs along the ring from where counting began to the executed person
        {
            float st = now - sweepTime;
            if (st >= 0 && st < 1.6f && sweepSpan > 0)
            {
                float grow = min(1.0f, st / 0.45f);
                grow = 1 - (1 - grow) * (1 - grow);   //ease-out
                float fade = (st < 0.9f) ? 1.0f : max(0.0f, 1.0f - (st - 0.9f) / 0.7f);
                drawSweep(window, centerX, centerY, radius - 2, radius + 2, sweepFrom, sweepSpan * grow,
                    sf::Color(cAccent.r, cAccent.g, cAccent.b, (sf::Uint8)(235 * fade)));
                float ha = sweepFrom + sweepSpan * grow;
                drawGlow(window, centerX + cos(ha) * radius, centerY + sin(ha) * radius, 16, cAccentHi, (int)(150 * fade));
            }
        }

        // people
        for (int i = 0; i < n; i++)
        {
            float cx = people[i].getPosition().x + personRadius;
            float cy = people[i].getPosition().y + personRadius;

            if (alive[i])
            {
                bool isCur = (i + 1 == currentPerson);
                bool isWin = (winner != 0 && i + 1 == winner);

                hi[i] += ((isCur ? 1.0f : 0.0f) - hi[i]) * min(1.0f, dt * 10.0f);   //eased highlight
                if (isWin)
                {
                    drawGlow(window, cx, cy, personRadius + 24, cSuccess, 120);
                }
                else if (hi[i] > 0.02f)
                {
                    drawGlow(window, cx, cy, personRadius + 24, cAccent, (int)(110 * hi[i]));
                }

                float fa = now - flashTime[i];   //subtle ring on people that were just counted
                if (fa >= 0 && fa < 0.5f)
                {
                    sf::CircleShape fl(personRadius + 4 + fa * 10);
                    fl.setPointCount(40);
                    fl.setOrigin(fl.getRadius(), fl.getRadius());
                    fl.setPosition(cx, cy);
                    fl.setFillColor(sf::Color::Transparent);
                    fl.setOutlineThickness(1.5f);
                    fl.setOutlineColor(sf::Color(cAccent.r, cAccent.g, cAccent.b, (sf::Uint8)(170 * (1 - fa / 0.5f))));
                    window.draw(fl);
                }

                if (isWin)
                {
                    people[i].setFillColor(cSuccess);
                    people[i].setOutlineColor(sf::Color(170, 235, 205));
                    people[i].setOutlineThickness(2);
                    labels[i].setFillColor(cBgTop);
                }
                else
                {
                    people[i].setFillColor(lerpColor(cNodeFill, cAccent, hi[i]));
                    people[i].setOutlineColor(lerpColor(cNodeBorder, cAccentHi, hi[i]));
                    people[i].setOutlineThickness(2);
                    labels[i].setFillColor(lerpColor(cText, cBgTop, hi[i]));
                }
                window.draw(people[i]);
                if (showLabels) window.draw(labels[i]);
            }
            else
            {
                hi[i] = 0;
                float age = now - deathTime[i];
                float mix = (age < 0) ? 1.0f : age / 0.4f;   //quick fade from red to dim
                if (i + 1 == lastExecuted)   //thin ring on the most recent execution
                {
                    sf::CircleShape lr(personRadius + 6);
                    lr.setPointCount(50);
                    lr.setOrigin(lr.getRadius(), lr.getRadius());
                    lr.setPosition(cx, cy);
                    lr.setFillColor(sf::Color::Transparent);
                    lr.setOutlineThickness(1.5f);
                    lr.setOutlineColor(sf::Color(cDanger.r, cDanger.g, cDanger.b, 170));
                    window.draw(lr);
                }
                people[i].setFillColor(lerpColor(cDanger, cDeadFill, mix));
                people[i].setOutlineColor(lerpColor(cDanger, cDeadBorder, mix));
                people[i].setOutlineThickness(2);
                window.draw(people[i]);
                labels[i].setFillColor(lerpColor(cText, sf::Color(104, 110, 128), mix));
                if (showLabels) window.draw(labels[i]);
                float d = personRadius * 0.62f;   //single thin strike-through
                drawLine(window, cx - d, cy + d, cx + d, cy - d, 1.5f, sf::Color(cDanger.r, cDanger.g, cDanger.b, 160));
            }
        }

        // callout tag for the current person when the circles are too small for numbers
        if (!showLabels)
        {
            int tag = (winner != 0) ? winner : currentPerson;
            float a = -PI_VAL / 2 + (tag - 1) * 2 * PI_VAL / n;
            float tx = centerX + cos(a) * (radius + personRadius + 38);
            float ty = centerY + sin(a) * (radius + personRadius + 38);
            sf::Color tc = (winner != 0) ? cSuccess : cAccent;
            drawBox(window, tx - 16, ty - 10, 32, 20, 10, tc, tc, 1);
            drawText(window, font, to_string(tag), 12, cBgTop, tx, ty - 1, 1, true);
        }

        // count badges: shows the 1..K counting of the last step (small badge pointing to the hub)
        if (n <= 40)
        {
            for (int i = 0; i < n; i++)
            {
                float fa = now - flashTime[i];
                if (fa >= 0 && fa < 0.9f && countNo[i] > 0)
                {
                    float a = -PI_VAL / 2 + i * 2 * PI_VAL / n;
                    float bx = centerX + cos(a) * (radius - personRadius - 14);
                    float by = centerY + sin(a) * (radius - personRadius - 14);
                    float fade = min(1.0f, (0.9f - fa) / 0.3f);
                    bool isKill = (!alive[i]);
                    sf::Color bc = isKill ? cDanger : cAccent;
                    sf::CircleShape badge(10);
                    badge.setPointCount(30);
                    badge.setOrigin(10, 10);
                    badge.setPosition(bx, by);
                    badge.setFillColor(sf::Color(13, 15, 20, (sf::Uint8)(235 * fade)));
                    badge.setOutlineThickness(1.5f);
                    badge.setOutlineColor(sf::Color(bc.r, bc.g, bc.b, (sf::Uint8)(255 * fade)));
                    window.draw(badge);
                    drawText(window, font, to_string(countNo[i]), 11, sf::Color(bc.r, bc.g, bc.b, (sf::Uint8)(255 * fade)), bx, by - 1, 1, true);
                }
            }
        }

        // center hub with progress ring
        int executedCount = n - q.getSize();
        int total = n - 1;
        float frac = (float)executedCount / total;
        if (winner != 0)
        {
            frac = 1.0f;
        }
        shownFrac += (frac - shownFrac) * min(1.0f, dt * 6.0f);
        sf::CircleShape hub(46);
        hub.setPointCount(80);
        hub.setOrigin(46, 46);
        hub.setPosition(centerX, centerY);
        hub.setFillColor(sf::Color(13, 15, 20));
        hub.setOutlineThickness(1);
        hub.setOutlineColor(cBorder);
        window.draw(hub);
        drawArc(window, centerX, centerY, 50, 53, 1.0f, sf::Color(36, 40, 52));
        drawArc(window, centerX, centerY, 50, 53, shownFrac, (winner != 0) ? cSuccess : cAccent);
        if (winner != 0)
        {
            drawText(window, font, "WINNER", 9, cSuccess, centerX, centerY - 15, 1, true);
            drawText(window, font, to_string(winner), 28, cText, centerX, centerY + 7, 1);
        }
        else
        {
            drawText(window, font, "REMAINING", 9, cDim, centerX, centerY - 15, 1, true);
            drawText(window, font, to_string(q.getSize()), 28, cText, centerX, centerY + 7, 1);
        }

        // ---- survivor card (fades in when the winner is found) ----
        if (winner != 0)
        {
            float wa = min(1.0f, (now - winnerTime) / 0.6f);
            float slide = (1 - wa) * 12;
            drawBox(window, 36, 530 + slide, 210, 70, 10, sf::Color(18, 32, 28, (sf::Uint8)(245 * wa)),
                sf::Color(cSuccess.r, cSuccess.g, cSuccess.b, (sf::Uint8)(255 * wa)), 1);
            drawText(window, font, "SURVIVOR", 9, sf::Color(cSuccess.r, cSuccess.g, cSuccess.b, (sf::Uint8)(255 * wa)), 52, 548 + slide, 0, true);
            drawText(window, font, "Person " + to_string(winner), 22, sf::Color(cText.r, cText.g, cText.b, (sf::Uint8)(255 * wa)), 52, 569 + slide, 0, true);
            drawText(window, font, "N = " + to_string(n) + ",  K = " + to_string(k) + ",  start " + to_string(s), 11,
                sf::Color(cDim.r, cDim.g, cDim.b, (sf::Uint8)(255 * wa)), 52, 589 + slide, 0);
        }

        // ---- queue strip (live view of the real queue) ----
        drawCard(window, font, 20, 636, 720, 84, "QUEUE");
        int shown = q.getSize();
        bool allShown = true;
        if (shown > 16)
        {
            shown = 16;
            allShown = false;
            drawText(window, font, "+ " + to_string(q.getSize() - 16) + " more", 12, cDim, 722, 654, 2);
        }
        for (int i = 0; i < shown; i++)
        {
            int v = q.getAt(i);
            float chipX = 36 + i * 43;
            if (i == 0)   //front of the queue
            {
                drawBox(window, chipX, 668, 34, 30, 6, cNodeFill, cAccent, 2);
                drawText(window, font, to_string(v), 13, cText, chipX + 17, 683, 1, true);
                drawText(window, font, "FRONT", 8, cAccent, chipX + 17, 709, 1, true);
            }
            else
            {
                drawBox(window, chipX, 668, 34, 30, 6, cNodeFill, sf::Color(58, 64, 82), 1);
                drawText(window, font, to_string(v), 13, cText, chipX + 17, 683, 1);
            }
            if (i == shown - 1 && allShown && shown > 1)
            {
                drawText(window, font, "REAR", 8, cDim, chipX + 17, 709, 1, true);
            }
        }

        // ---- KPI cards ----
        string kpiLabel[3] = { "PEOPLE (N)", "SKIP (K)", "START" };
        string kpiValue[3] = { to_string(n), to_string(k), to_string(s) };
        for (int i = 0; i < 3; i++)
        {
            float cx = 760 + i * 110;
            drawCard(window, font, cx, 76, 100, 76, "");
            drawText(window, font, kpiLabel[i], 10, cDim, cx + 16, 96, 0, true);
            drawText(window, font, kpiValue[i], 26, (i == 2) ? cAccent : cText, cx + 16, 126, 0);
        }

        // ---- status card ----
        string stateName = "READY";
        sf::Color stateColor = cDim;
        if (winner != 0)
        {
            stateName = "FINISHED";
            stateColor = cSuccess;
        }
        else if (started)
        {
            stateName = "RUNNING";
            stateColor = cAccent;
        }
        else if (orderCount > 0)
        {
            stateName = "PAUSED";
            stateColor = cAmber;
        }
        drawCard(window, font, 760, 166, 320, 84, "STATUS");
        drawBox(window, 984, 174, 80, 20, 10, sf::Color(stateColor.r / 6 + 14, stateColor.g / 6 + 14, stateColor.b / 6 + 16), stateColor, 1);
        drawText(window, font, stateName, 10, stateColor, 1024, 183, 1, true);
        drawText(window, font, statusString, 15, cText, 776, 205, 0);
        drawText(window, font, "Step " + to_string(orderCount) + " of " + to_string(n - 1) + "   |   " +
            to_string(q.getSize()) + " remaining", 11, cDim, 776, 226, 0);
        drawRect(window, 776, 243, 288, 3, sf::Color(36, 40, 52));
        if (shownFrac > 0)
        {
            drawRect(window, 776, 243, 288 * shownFrac, 3, (winner != 0) ? cSuccess : cAccent);
        }

        // ---- elimination log ----
        // ---- queue operations (shows the work the queue did: about N x K) ----
        drawCard(window, font, 760, 264, 320, 76, "QUEUE OPERATIONS");
        string opLabel[3] = { "ENQUEUE", "DEQUEUE", "TOTAL" };
        string opValue[3] = { to_string(enqOps), to_string(deqOps), to_string(enqOps + deqOps) };
        for (int i = 0; i < 3; i++)
        {
            float ox = 776 + i * 96;
            drawText(window, font, opLabel[i], 9, cDim, ox, 306, 0, true);
            drawText(window, font, opValue[i], 16, (i == 2) ? cAccent : cText, ox, 324, 0, true);
        }

        drawCard(window, font, 760, 354, 320, 266, "ELIMINATION LOG");
        drawText(window, font, "ROUND", 9, cDim, 776, 396, 0, true);
        drawText(window, font, "PERSON", 9, cDim, 1064, 396, 2, true);
        drawRect(window, 776, 407, 288, 1, cBorder);
        if (orderCount == 0)
        {
            drawText(window, font, "No eliminations yet", 13, cDim, 920, 505, 1);
        }
        for (int r = 0; r < 9; r++)
        {
            int j = orderCount - 1 - r;   //newest first
            if (j < 0)
            {
                break;
            }
            float ry = 420 + r * 22;
            if (r % 2 == 0)
            {
                drawRect(window, 768, ry - 11, 304, 22, sf::Color(255, 255, 255, 6));
            }
            if (r == 0)
            {
                drawRect(window, 768, ry - 11, 2, 22, cDanger);
            }
            string rn = to_string(j + 1);
            if (rn.size() < 2)
            {
                rn = "0" + rn;
            }
            drawText(window, font, rn, 12, cDim, 780, ry - 1, 0);
            drawText(window, font, to_string(order[j]), 13, (r == 0) ? cDanger : cText, 1064, ry - 1, 2, r == 0);
        }

        // ---- buttons ----
        drawButton(window, font, startRect, "START", true, startRect.contains(mouse), started);
        drawButton(window, font, pauseRect, "PAUSE", false, pauseRect.contains(mouse),
            (!started && orderCount > 0 && winner == 0));
        drawButton(window, font, resetRect, "RESET", false, resetRect.contains(mouse), false);
        drawText(window, font, "Space: start / pause     R: reset     S: snapshot", 11, cDim, 920, 700, 1);

        // ---- snapshot toast ----
        {
            float tt = now - toastTime;
            if (tt >= 0 && tt < 1.8f)
            {
                float al = (tt < 1.3f) ? 1.0f : (1.8f - tt) / 0.5f;
                drawBox(window, 280, 92, 200, 26, 13, sf::Color(24, 27, 36, (sf::Uint8)(240 * al)),
                    sf::Color(cAccent.r, cAccent.g, cAccent.b, (sf::Uint8)(255 * al)), 1);
                drawText(window, font, "Snapshot saved", 12, sf::Color(cText.r, cText.g, cText.b, (sf::Uint8)(255 * al)), 380, 104, 1, true);
            }
        }

        // ---- hover tooltip (info about the person under the mouse) ----
        {
            int hov = -1;
            for (int i = 0; i < n; i++)
            {
                float hx = people[i].getPosition().x + personRadius - mouse.x;
                float hy = people[i].getPosition().y + personRadius - mouse.y;
                float hr = max(personRadius + 4, 8.0f);
                if (hx * hx + hy * hy <= hr * hr)
                {
                    hov = i;
                    break;
                }
            }
            if (hov >= 0)
            {
                string l2, l3;
                sf::Color c2 = cAccent;
                if (alive[hov])
                {
                    if (winner != 0 && hov + 1 == winner)
                    {
                        l2 = "Survivor";
                        c2 = cSuccess;
                    }
                    else
                    {
                        l2 = (hov + 1 == currentPerson) ? "Alive  -  counting starts here" : "Alive";
                    }
                    for (int j = 0; j < q.getSize(); j++)
                    {
                        if (q.getAt(j) == hov + 1)
                        {
                            l3 = "Queue position " + to_string(j + 1) + " of " + to_string(q.getSize());
                            break;
                        }
                    }
                }
                else
                {
                    l2 = "Eliminated";
                    c2 = cDanger;
                    for (int j = 0; j < orderCount; j++)
                    {
                        if (order[j] == hov + 1)
                        {
                            l3 = "Round " + to_string(j + 1) + " of " + to_string(n - 1);
                            break;
                        }
                    }
                }
                float bw = 210;
                float bh = 62;
                float bx = mouse.x + 16;
                float by = mouse.y + 16;
                if (bx + bw > W - 8)
                {
                    bx = mouse.x - bw - 16;
                }
                if (by + bh > H - 8)
                {
                    by = mouse.y - bh - 16;
                }
                drawRound(window, bx + 2, by + 4, bw, bh, 8, sf::Color(0, 0, 0, 110));
                drawBox(window, bx, by, bw, bh, 8, sf::Color(24, 27, 36), cBorder, 1);
                drawText(window, font, "Person " + to_string(hov + 1), 13, cText, bx + 12, by + 16, 0, true);
                drawText(window, font, l2, 12, c2, bx + 12, by + 34, 0);
                drawText(window, font, l3, 11, cDim, bx + 12, by + 50, 0);
            }
        }

        window.display();
    }
    delete[]people;
    delete[]labels;
    delete[] alive;
    delete[] order;
    delete[] deathTime;
    delete[] flashTime;
    delete[] countNo;
    delete[] hi;
    return 0;
}
