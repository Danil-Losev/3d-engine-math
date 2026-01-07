# Отчёт по оптимизации vec3.cpp

## Текущая производительность
```
Vector addition:         5165.07 ms ( 193.608 M ops/s)
Vector subtraction:      5284.78 ms ( 189.222 M ops/s)
Scalar multiply:         5516.53 ms ( 181.273 M ops/s)
Dot product:             2256.45 ms ( 443.173 M ops/s)
Cross product:           6730.52 ms ( 148.577 M ops/s)
Length:                  2786.15 ms ( 358.918 M ops/s)
Normalize:               9601.57 ms (  104.15 M ops/s)
Distance:                   3755 ms ( 266.312 M ops/s)
Projection:                10112 ms ( 98.8927 M ops/s)
Reflection:                15611 ms ( 64.0573 M ops/s)
```

---

## 1. Vector Addition / Subtraction (operator+, operator-)
**Текущий код (строки 46-60):**
```cpp
vec3 vec3::operator+(const vec3 &other) const {
    vec3 result;  // Вызывает конструктор по умолчанию
    result.m_x = m_x + other.m_x;
    result.m_y = m_y + other.m_y;
    result.m_z = m_z + other.m_z;
    return result;
}
```

### Проблемы:
1. **Двойная инициализация**: `vec3 result;` инициализирует все поля в 0.0f, затем они перезаписываются
2. **Неэффективное использование конструктора**: можно напрямую создать объект с нужными значениями

### Оптимизация:
```cpp
vec3 vec3::operator+(const vec3 &other) const {
    return vec3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

vec3 vec3::operator-(const vec3 &other) const {
    return vec3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}
```

### Почему быстрее:
- **Убрана двойная инициализация**: создаётся объект сразу с нужными значениями
- **Меньше присваиваний**: 0 присваиваний вместо 3
- **RVO (Return Value Optimization)**: компилятор может эффективнее оптимизировать создание временного объекта напрямую в месте назначения

**Ожидаемое ускорение: 15-30%** (до ~220-250 M ops/s)

---

## 2. Scalar Multiply (operator*)
**Текущий код (строки 76-82):**
```cpp
vec3 vec3::operator*(float scalar) const {
    vec3 result;
    result.m_x = m_x * scalar;
    result.m_y = m_y * scalar;
    result.m_z = m_z * scalar;
    return result;
}
```

### Проблемы:
- Такая же проблема двойной инициализации

### Оптимизация:
```cpp
vec3 vec3::operator*(float scalar) const {
    return vec3(m_x * scalar, m_y * scalar, m_z * scalar);
}
```

**Ожидаемое ускорение: 20-35%** (до ~230-245 M ops/s)

---

## 3. Cross Product (cross_production)
**Текущий код (строки 90-96):**
```cpp
vec3 vec3::cross_production(const vec3 &other) const {
    vec3 result;
    result.m_x = m_y * other.m_z - m_z * other.m_y;
    result.m_y = m_z * other.m_x - m_x * other.m_z;
    result.m_z = m_x * other.m_y - m_y * other.m_x;
    return result;
}
```

### Проблемы:
- Двойная инициализация

### Оптимизация:
```cpp
vec3 vec3::cross_production(const vec3 &other) const {
    return vec3(
        m_y * other.m_z - m_z * other.m_y,
        m_z * other.m_x - m_x * other.m_z,
        m_x * other.m_y - m_y * other.m_x
    );
}
```

**Ожидаемое ускорение: 15-25%** (до ~170-185 M ops/s)

---

## 4. Normalize (normalize)
**Текущий код (строки 107-114):**
```cpp
vec3 vec3::normalize() const {
    float len = m_x * m_x + m_y * m_y + m_z * m_z;
    if (len < 1e-8f) {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float inv_len = 1.0f / std::sqrt(len);
    return vec3(m_x * inv_len, m_y * inv_len, m_z * inv_len);
}
```

### Проблемы:
1. **Проверка близости к нулю**: `if (len < 1e-8f)` выполняется при каждом вызове, но нулевой вектор — редкий случай
2. **Branch prediction miss**: ветвление замедляет pipeline CPU

### Оптимизация:
```cpp
vec3 vec3::normalize() const {
    float len_sq = m_x * m_x + m_y * m_y + m_z * m_z;
    // Избегаем ветвления: если len_sq очень мал, inv_len будет огромным,
    // но результат всё равно будет близок к нулю после умножения
    float inv_len = 1.0f / std::sqrt(len_sq + 1e-30f);
    return vec3(m_x * inv_len, m_y * inv_len, m_z * inv_len);
}
```

**Альтернатива (если точность критична):**
```cpp
vec3 vec3::normalize() const {
    float len_sq = m_x * m_x + m_y * m_y + m_z * m_z;
    if (len_sq < 1e-8f) [[unlikely]] {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    return vec3(m_x * inv_len, m_y * inv_len, m_z * inv_len);
}
```

### Почему быстрее:
- **Без ветвления**: CPU может эффективнее выполнять последовательные инструкции
- **Атрибут `[[unlikely]]`**: подсказывает компилятору оптимизировать основной путь выполнения
- **Добавление эпсилон к знаменателю**: избегает деления на ноль без проверки

**Ожидаемое ускорение: 40-60%** (до ~145-165 M ops/s)

---

## 5. Distance (distance_to)
**Текущий код (строки 98-103):**
```cpp
float vec3::distance_to(const vec3 &other) const {
    float dx = m_x - other.m_x;
    float dy = m_y - other.m_y;
    float dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
```

### Проблемы:
- **Лишние переменные**: создание временных переменных `dx`, `dy`, `dz` может мешать оптимизации компилятора
- Современные компиляторы обычно оптимизируют это, но можно улучшить

### Оптимизация:
```cpp
float vec3::distance_to(const vec3 &other) const {
    const float dx = m_x - other.m_x;
    const float dy = m_y - other.m_y;
    const float dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
```

**Примечание:** добавление `const` помогает компилятору понять, что значения не изменяются, что может улучшить регистровую аллокацию. Альтернативно, можно сделать inline-вычисление:

```cpp
float vec3::distance_to(const vec3 &other) const {
    return std::sqrt((m_x - other.m_x) * (m_x - other.m_x) +
                     (m_y - other.m_y) * (m_y - other.m_y) +
                     (m_z - other.m_z) * (m_z - other.m_z));
}
```

**Ожидаемое ускорение: 5-10%** (до ~280-295 M ops/s) - минимальное, т.к. код уже близок к оптимальному

---

## 6. Projection (project_on_vector)
**Текущий код (строки 163-171):**
```cpp
vec3 vec3::project_on_vector(const vec3 &other) const {
    float dot_p = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z;
    if (other_len_sq < 1e-8f) {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    float scalar = dot_p / other_len_sq;
    return vec3(other.m_x * scalar, other.m_y * scalar, other.m_z * scalar);
}
```

### Проблемы:
1. **Ветвление** для проверки нулевого вектора
2. **Деление**: медленнее умножения

### Оптимизация:
```cpp
vec3 vec3::project_on_vector(const vec3 &other) const {
    const float dot_p = m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    const float other_len_sq = other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z;
    const float inv_len_sq = 1.0f / (other_len_sq + 1e-30f);
    const float scalar = dot_p * inv_len_sq;
    return vec3(other.m_x * scalar, other.m_y * scalar, other.m_z * scalar);
}
```

### Почему быстрее:
- **Убрано ветвление**: добавление эпсилон предотвращает деление на ноль
- **Умножение вместо деления**: `1.0f / x` вычисляется один раз, затем умножение на `inv_len_sq`

**Ожидаемое ускорение: 35-50%** (до ~135-150 M ops/s)

---

## 7. Reflection (reflect)
**Текущий код (строки 130-144):**
```cpp
vec3 vec3::reflect(const vec3 &normal) const {
    float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z;
    if (len_sq < 1e-8f) {
        return *this;
    }
    float inv_len = 1.0f / std::sqrt(len_sq);
    float nx = normal.m_x * inv_len;
    float ny = normal.m_y * inv_len;
    float nz = normal.m_z * inv_len;

    float dot = m_x * nx + m_y * ny + m_z * nz;
    float factor = 2.0f * dot;

    return vec3(m_x - nx * factor, m_y - ny * factor, m_z - nz * factor);
}
```

### Проблемы:
1. **Ветвление** для проверки нулевой нормали
2. **Лишние временные переменные** для нормализованной нормали
3. **sqrt + деление**: можно оптимизировать

### Оптимизация:
```cpp
vec3 vec3::reflect(const vec3 &normal) const {
    const float len_sq = normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z;
    const float inv_len = 1.0f / std::sqrt(len_sq + 1e-30f);
    
    const float nx = normal.m_x * inv_len;
    const float ny = normal.m_y * inv_len;
    const float nz = normal.m_z * inv_len;
    
    const float factor = 2.0f * (m_x * nx + m_y * ny + m_z * nz);
    
    return vec3(m_x - nx * factor, m_y - ny * factor, m_z - nz * factor);
}
```

### Почему быстрее:
- **Убрано ветвление**: эпсилон предотвращает деление на ноль
- **Меньше промежуточных переменных**: `dot` и `factor` объединены
- **const переменные**: помогают компилятору оптимизировать

**Ожидаемое ускорение: 40-60%** (до ~90-105 M ops/s)

---

## 8. Дополнительные улучшения

### 8.1 Конструктор (строки 8-15)
**Текущий код:**
```cpp
vec3::vec3(float x, float y, float z, bool normalize) {
    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
    if (normalize) {
        this->to_normalized();
    }
}
```

**Оптимизация:**
```cpp
vec3::vec3(float x, float y, float z, bool normalize) : m_x(x), m_y(y), m_z(z) {
    if (normalize) [[unlikely]] {
        to_normalized();
    }
}
```

### Почему быстрее:
- **Список инициализации**: прямая инициализация членов без присваивания
- **Убран `this->`**: не влияет на производительность, но улучшает читаемость
- **`[[unlikely]]`**: указывает компилятору, что нормализация редко используется

---

### 8.2 Static factory методы (строки 173-181)
**Текущий код:**
```cpp
vec3 vec3::zero() { return vec3(0.0f, 0.0f, 0.0f); }
vec3 vec3::one() { return vec3(1.0f, 1.0f, 1.0f); }
vec3 vec3::basis_i() { return vec3(1.0f, 0.0f, 0.0f); }
vec3 vec3::basis_j() { return vec3(0.0f, 1.0f, 0.0f); }
vec3 vec3::basis_k() { return vec3(0.0f, 0.0f, 1.0f); }
```

**Оптимизация:** добавить `constexpr` и `inline`
```cpp
static constexpr vec3 zero() noexcept { return vec3(0.0f, 0.0f, 0.0f); }
static constexpr vec3 one() noexcept { return vec3(1.0f, 1.0f, 1.0f); }
static constexpr vec3 basis_i() noexcept { return vec3(1.0f, 0.0f, 0.0f); }
static constexpr vec3 basis_j() noexcept { return vec3(0.0f, 1.0f, 0.0f); }
static constexpr vec3 basis_k() noexcept { return vec3(0.0f, 0.0f, 1.0f); }
```

**Примечание:** требует сделать конструктор `constexpr`.

---

### 8.3 Static cross_production (строки 226-232)
Та же оптимизация, что и для member-функции:
```cpp
vec3 vec3::cross_production(const vec3 &a, const vec3 &b) {
    return vec3(
        a.m_y * b.m_z - a.m_z * b.m_y,
        a.m_z * b.m_x - a.m_x * b.m_z,
        a.m_x * b.m_y - a.m_y * b.m_x
    );
}
```

---

## 9. Компиляционные оптимизации

### 9.1 Флаги компиляции
Убедитесь, что используются следующие флаги:
```bash
g++ -O3 -march=native -ffast-math -flto main.cpp vec3.cpp -o main.exe
```

- **`-O3`**: максимальная оптимизация
- **`-march=native`**: оптимизация под вашу архитектуру CPU (использует SSE/AVX инструкции)
- **`-ffast-math`**: разрешает небезопасные математические оптимизации (быстрее, но менее точно)
- **`-flto`**: Link Time Optimization - оптимизация во время линковки

### 9.2 Inline функции
Переместить короткие функции в header файл с `inline`:
```cpp
inline vec3 vec3::operator+(const vec3 &other) const {
    return vec3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

inline float vec3::dot_production(const vec3 &other) const {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}
```

---

## 10. Ожидаемые результаты после всех оптимизаций

| Операция               | Сейчас (M ops/s) | Ожидаемо (M ops/s) | Ускорение |
|------------------------|------------------|--------------------|-----------|
| Vector addition        | 193.6            | 220-250            | ~20-30%   |
| Vector subtraction     | 189.2            | 215-245            | ~20-30%   |
| Scalar multiply        | 181.3            | 230-245            | ~25-35%   |
| Dot product            | 443.2            | 450-500            | ~5-15%    |
| Cross product          | 148.6            | 170-185            | ~15-25%   |
| Length                 | 358.9            | 360-380            | ~5-10%    |
| Normalize              | 104.2            | 145-165            | ~40-60%   |
| Distance               | 266.3            | 280-295            | ~5-10%    |
| Projection             | 98.9             | 135-150            | ~35-50%   |
| Reflection             | 64.1             | 90-105             | ~40-60%   |

---

## 11. Резюме ключевых принципов оптимизации

1. **Избегайте двойной инициализации**: используйте конструктор напрямую вместо создания объекта по умолчанию + присваивание
2. **Убирайте ветвления**: используйте математические трюки (добавление эпсилон) или атрибут `[[unlikely]]`
3. **Замените деление на умножение**: `x / y` → `x * (1.0f / y)`
4. **Используйте const**: помогает компилятору оптимизировать регистры
5. **Inline короткие функции**: избегайте overhead вызова функций
6. **Оптимизация компиляции**: `-O3 -march=native -ffast-math`
7. **Список инициализации**: предпочтительнее присваивания в теле конструктора

---

## 12. Следующие шаги

1. Применить все оптимизации к `vec3.cpp`
2. Перекомпилировать с флагами `-O3 -march=native -ffast-math -flto`
3. Запустить тесты производительности заново
4. Сравнить результаты

**Общее ожидаемое ускорение: 20-50% в зависимости от операции**
