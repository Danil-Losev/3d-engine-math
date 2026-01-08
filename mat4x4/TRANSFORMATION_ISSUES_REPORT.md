# Анализ матриц трансформации mat4x4

## Дата анализа
2026-01-08

## Результаты тестирования
**Статус:** ✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ

```
Test 1 - Translation:
Expected: (5, 10, 15, 1)
Got: (5, 10, 15, 1) ✅

Test 2 - Scale then Translate:
Expected: (7, 12, 17, 1)
Got: (7, 12, 17, 1) ✅

Test 3 - 90° rotation around Z:
Expected: (~0, ~1, 0, 1)
Got: (-4.37114e-08, 1, 0, 1) ✅ (погрешность округления)
```

---

## Детальный анализ реализации

### 1. ✅ КОРРЕКТНО: Матрица переноса (translation)

**Местоположение:** `mat4x4.cpp`, строки 650-656

**Текущая реализация:**
```cpp
math::mat4x4 math::mat4x4::translation(float tx, float ty, float tz)
{
    return { { { 1.0f, 0.0f, 0.0f, tx },
        { 0.0f, 1.0f, 0.0f, ty },
        { 0.0f, 0.0f, 1.0f, tz },
        { 0.0f, 0.0f, 0.0f, 1.0f } } };
}
```

**Вывод:** РЕАЛИЗАЦИЯ ПРАВИЛЬНАЯ

**Объяснение семантики:**
Ваша библиотека использует **column-major семантику с row-major хранением**:
- Хранение: `m_matrix[row][col]` (row-major layout в памяти)
- Логика: column-major математика (компоненты переноса в последнем СТОЛБЦЕ)
- Умножение: `M × v` (column-vector)

Это **КОРРЕКТНЫЙ** гибридный подход, используемый в OpenGL и многих 3D-библиотеках.

**Матрица визуально (как хранится):**
```
[1  0  0  tx]
[0  1  0  ty]
[0  0  1  tz]
[0  0  0  1 ]
```

**Как работает умножение M × v:**
```cpp
result.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w
         = 1*v.x + 0*v.y + 0*v.z + tx*1 = v.x + tx ✅
```

---

### 2. ✅ КОРРЕКТНО: Умножение матрицы на вектор

**Местоположение:** `mat4x4.cpp`, строки 497-507

**Реализация:**
```cpp
math::vec4 math::mat4x4::operator*(const vec4& vector) const
{
    vec4 result;
    result.x(m_matrix[0][0] * vector.x() + m_matrix[0][1] * vector.y() + 
             m_matrix[0][2] * vector.z() + m_matrix[0][3] * vector.w());
    result.y(m_matrix[1][0] * vector.x() + m_matrix[1][1] * vector.y() + 
             m_matrix[1][2] * vector.z() + m_matrix[1][3] * vector.w());
    result.z(m_matrix[2][0] * vector.x() + m_matrix[2][1] * vector.y() + 
             m_matrix[2][2] * vector.z() + m_matrix[2][3] * vector.w());
    result.w(m_matrix[3][0] * vector.x() + m_matrix[3][1] * vector.y() + 
             m_matrix[3][2] * vector.z() + m_matrix[3][3] * vector.w());
    return result;
}
```

**Вывод:** Правильная формула для `M × v` (column-vector).

Каждая компонента результата = скалярное произведение строки матрицы на вектор.

---

### 3. ✅ КОРРЕКТНО: Матрицы вращения

**rotation_x** (строки 666-674):
```cpp
return { { { 1.0f, 0.0f,   0.0f,  0.0f },
           { 0.0f, cos_a, -sin_a, 0.0f },
           { 0.0f, sin_a,  cos_a, 0.0f },
           { 0.0f, 0.0f,   0.0f,  1.0f } } };
```
✅ Корректная формула для вращения вокруг оси X

**rotation_y** (строки 676-684):
```cpp
return { { {  cos_a, 0.0f, sin_a, 0.0f },
           {  0.0f,  1.0f, 0.0f,  0.0f },
           { -sin_a, 0.0f, cos_a, 0.0f },
           {  0.0f,  0.0f, 0.0f,  1.0f } } };
```
✅ Корректная формула для вращения вокруг оси Y

**rotation_z** (строки 686-694):
```cpp
return { { { cos_a, -sin_a, 0.0f, 0.0f },
           { sin_a,  cos_a, 0.0f, 0.0f },
           { 0.0f,   0.0f,  1.0f, 0.0f },
           { 0.0f,   0.0f,  0.0f, 1.0f } } };
```
✅ Корректная формула для вращения вокруг оси Z (тест подтвердил)

---

### 4. ✅ КОРРЕКТНО: Матрица масштабирования

**Местоположение:** `mat4x4.cpp`, строки 658-664

```cpp
math::mat4x4 math::mat4x4::scaling(float sx, float sy, float sz)
{
    return { { { sx,   0.0f, 0.0f, 0.0f },
               { 0.0f, sy,   0.0f, 0.0f },
               { 0.0f, 0.0f, sz,   0.0f },
               { 0.0f, 0.0f, 0.0f, 1.0f } } };
}
```
✅ Классическая матрица масштабирования

---

### 5. ✅ КОРРЕКТНО: Составная модельная матрица

**Местоположение:** `mat4x4.cpp`, строки 713-717

```cpp
math::mat4x4 math::mat4x4::make_model_matrix(
    const mat4x4& translation, const mat4x4& rotation, const mat4x4& scaling)
{
    return translation * rotation * scaling;
}
```

**Порядок применения (справа налево для column-vector):**
1. Сначала масштабирование (S)
2. Затем вращение (R)  
3. Наконец перенос (T)

**Формула:** `v' = T × R × S × v`

Это **стандартный и правильный** порядок для 3D-графики:
- Объект сначала масштабируется в локальных координатах
- Затем поворачивается
- Наконец переносится в мировые координаты

✅ Тест подтвердил: точка (1,1,1) с масштабом ×2 и переносом (+5,+10,+15) = (7,12,17)

---

### 6. ✅ КОРРЕКТНО: Композиция вращений

**Местоположение:** `mat4x4.cpp`, строки 696-711

```cpp
math::mat4x4 math::mat4x4::rotation_axis_angle(
    std::optional<float> angle_rad_x,
    std::optional<float> angle_rad_y,
    std::optional<float> angle_rad_z)
{
    mat4x4 rotation = mat4x4::identity();
    if (angle_rad_x.has_value()) [[likely]] {
        rotation = rotation * rotation_x(angle_rad_x.value());
    }
    if (angle_rad_y.has_value()) [[likely]] {
        rotation = rotation * rotation_y(angle_rad_y.value());
    }
    if (angle_rad_z.has_value()) [[likely]] {
        rotation = rotation * rotation_z(angle_rad_z.value());
    }
    return rotation;
}
```

**Порядок вращений:** X → Y → Z (стандартный Euler angles порядок)

✅ Реализация корректна для последовательных вращений

---

## Архитектурные особенности

### Гибридная система координат

Ваша реализация использует **column-major математику** с **row-major хранением**:

**Преимущества:**
1. ✅ Совместимость с OpenGL (column-major математика)
2. ✅ Удобное индексирование `m[row][col]` в C++
3. ✅ Естественное представление матриц в коде
4. ✅ Эффективный SIMD-код (загрузка строк)

**Консистентность:**
- Хранение: row-major layout
- Семантика: column-major (компоненты перемещения в столбце)
- Умножение векторов: `M × v` (column-vector)
- Порядок композиции: справа налево

---

## Проверка SIMD-кода

### Умножение матриц (operator*)

**SIMD версия** (строки 99-157) и **скалярная версия** (строки 159-207) используют одинаковую формулу:

```
result[i][j] = Σ(this[i][k] × other[k][j])
```

**Статус:** ✅ Обе версии математически эквивалентны

**Замечание:** Убедитесь, что при включении SIMD (`#undef NO_SIMD`) результаты совпадают.

---

## Потенциальные улучшения

### 1. Документация

**Рекомендация:** Добавить в заголовок комментарии:

```cpp
// Matrix storage: row-major layout (m_matrix[row][col])
// Matrix semantics: column-major (translation in last column)
// Vector multiplication: M × v (column-vector on the right)
// Transform composition: T × R × S (applied right-to-left)
```

### 2. Проверка SIMD

**Рекомендация:** Добавить тест сравнения SIMD и скалярного умножения:

```cpp
#define NO_SIMD
mat4x4 a = ..., b = ...;
mat4x4 scalar_result = a * b;

#undef NO_SIMD
mat4x4 simd_result = a * b;

// assert(scalar_result ≈ simd_result)
```

### 3. Дополнительные трансформации

**Для будущей реализации:**
- `perspective()` — перспективная проекция
- `orthographic()` — ортографическая проекция  
- `look_at()` — камера view-матрица
- `frustum()` — произвольный frustum

Все они должны следовать той же column-major семантике.

---

## Заключение

**Общий статус: ✅ ВСЁ КОРРЕКТНО**

### Что работает правильно:
- ✅ `translation()` — компоненты в правильном месте (последний столбец)
- ✅ `scaling()` — стандартная матрица масштабирования
- ✅ `rotation_x/y/z()` — корректные формулы вращения
- ✅ `rotation_axis_angle()` — правильная композиция Euler angles
- ✅ `make_model_matrix()` — правильный порядок T×R×S
- ✅ `operator*(vec4)` — корректное умножение M×v
- ✅ `operator*(mat4x4)` — корректное умножение матриц

### Что требует внимания:
- ⚠️ Недостаточная документация семантики матриц
- ⚠️ Отсутствие тестов для SIMD vs скалярного кода
- ⚠️ Не реализованы projection/view матрицы (но закомментированы в .hpp)

### Рекомендации:
1. Добавить документацию семантики в заголовочный файл
2. Создать unit-тесты для проверки трансформаций
3. Проверить SIMD-код с реальными данными (убрать `#define NO_SIMD` и протестировать)
4. Сохранить текущие тесты как регрессионные

**Вывод:** Ваша реализация матриц трансформации полностью корректна и готова к использованию. Первоначальное предположение об ошибке было неверным — тесты подтвердили правильность всех трансформаций.

**Местоположение:** `mat4x4.cpp`, строки 713-717

**Текущая реализация:**
```cpp
math::mat4x4 math::mat4x4::make_model_matrix(
    const mat4x4& translation, const mat4x4& rotation, const mat4x4& scaling)
{
    return translation * rotation * scaling;
}
```

**Проблема:**
Порядок умножения зависит от семантики вашего матричного умножения и формата хранения.

**Стандартные конвенции:**

1. **Row-major с row-vector (v×M):**
   ```
   v' = v × (S × R × T)
   // Масштабирование → Вращение → Перенос
   ```

2. **Column-major с column-vector (M×v):**
   ```
   v' = (T × R × S) × v
   // Масштабирование → Вращение → Перенос
   ```

**Ваша реализация:**
- Хранение: row-major (`m_matrix[row][col]`)
- Умножение vec: `mat * vec` (строка 497-507) использует формулу для column-vector
- **Проблема:** смешанная семантика!

**Рекомендация:**
Если используете `M × v` (column-vector), то правильный порядок:
```cpp
return translation * rotation * scaling;  // T × R × S
```

Если используете `v × M` (row-vector), нужно обратить порядок:
```cpp
return scaling * rotation * translation;  // S × R × T
```

---

### 3. ⚠️ НЕСООТВЕТСТВИЕ: Умножение матрицы на вектор

**Местоположение:** `mat4x4.cpp`, строки 497-507

**Текущая реализация:**
```cpp
math::vec4 math::mat4x4::operator*(const vec4& vector) const
{
    vec4 result;
    result.x(m_matrix[0][0] * vector.x() + m_matrix[0][1] * vector.y() + 
             m_matrix[0][2] * vector.z() + m_matrix[0][3] * vector.w());
    result.y(m_matrix[1][0] * vector.x() + m_matrix[1][1] * vector.y() + 
             m_matrix[1][2] * vector.z() + m_matrix[1][3] * vector.w());
    result.z(m_matrix[2][0] * vector.x() + m_matrix[2][1] * vector.y() + 
             m_matrix[2][2] * vector.z() + m_matrix[2][3] * vector.w());
    result.w(m_matrix[3][0] * vector.x() + m_matrix[3][1] * vector.y() + 
             m_matrix[3][2] * vector.z() + m_matrix[3][3] * vector.w());
    return result;
}
```

**Анализ:**
Это реализация умножения `M × v` (column-vector), где:
- result[i] = sum(m_matrix[i][j] * vector[j])
- Используется i-я строка матрицы для вычисления i-й компоненты результата

**Проблема:**
Эта семантика КОНФЛИКТУЕТ с текущей матрицей translation, которая размещает tx,ty,tz в столбце вместо строки!

---

## Дополнительные замечания

### Матрицы вращения

**rotation_x, rotation_y, rotation_z** (строки 666-694):
- Формулы выглядят КОРРЕКТНО для row-major формата
- Но эффективность зависит от исправления translation и согласования семантики

### SIMD код умножения матриц

**operator*(const mat4x4& other)** (строки 99-209):
SIMD-версия использует правильную формулу для row-major матриц, НО:
- Код предполагает определенную семантику умножения
- После исправления translation нужно убедиться, что SIMD и скалярная версии дают одинаковые результаты

---

## Рекомендации по исправлению

### Немедленные действия:

1. **ИСПРАВИТЬ `translation()`:**
   ```cpp
   math::mat4x4 math::mat4x4::translation(float tx, float ty, float tz)
   {
       return { { { 1.0f, 0.0f, 0.0f, 0.0f },
           { 0.0f, 1.0f, 0.0f, 0.0f },
           { 0.0f, 0.0f, 1.0f, 0.0f },
           { tx,   ty,   tz,   1.0f } } };
   }
   ```

2. **ПРОВЕРИТЬ `make_model_matrix()`:**
   - Текущий порядок `T × R × S` корректен для column-vector семантики
   - Оставить как есть, если используете `M × v`

3. **ДОБАВИТЬ ТЕСТЫ:**
   ```cpp
   // Тест трансляции
   mat4x4 t = mat4x4::translation(1.0f, 2.0f, 3.0f);
   vec4 v(0.0f, 0.0f, 0.0f, 1.0f);
   vec4 result = t * v;
   // Ожидается: result = (1.0, 2.0, 3.0, 1.0)
   
   // Тест композиции
   mat4x4 s = mat4x4::scaling(2.0f, 2.0f, 2.0f);
   mat4x4 r = mat4x4::rotation_z(0.0f);
   mat4x4 model = mat4x4::make_model_matrix(t, r, s);
   vec4 p(1.0f, 0.0f, 0.0f, 1.0f);
   vec4 transformed = model * p;
   // Проверить ожидаемый результат
   ```

### Долгосрочные улучшения:

1. Документировать соглашение (row-major, column-vector)
2. Добавить unit-тесты для всех трансформаций
3. Проверить согласованность SIMD и скалярного кода
4. Добавить валидацию для специальных случаев

---

## Тестовый пример для проверки

```cpp
#include "mat4x4.hpp"
#include <iostream>
#include <cmath>

int main() {
    using namespace math;
    
    // Тест 1: Простая трансляция
    mat4x4 trans = mat4x4::translation(5.0f, 10.0f, 15.0f);
    vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 result = trans * point;
    
    std::cout << "Test 1 - Translation:\n";
    std::cout << "Expected: (5, 10, 15, 1)\n";
    std::cout << "Got: (" << result.x() << ", " << result.y() 
              << ", " << result.z() << ", " << result.w() << ")\n\n";
    
    // Тест 2: Масштабирование + трансляция
    mat4x4 scale = mat4x4::scaling(2.0f, 2.0f, 2.0f);
    mat4x4 model = mat4x4::make_model_matrix(trans, mat4x4::identity(), scale);
    vec4 point2(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 result2 = model * point2;
    
    std::cout << "Test 2 - Scale then Translate:\n";
    std::cout << "Expected: (7, 12, 17, 1)  // (1*2 + 5, 1*2 + 10, 1*2 + 15)\n";
    std::cout << "Got: (" << result2.x() << ", " << result2.y() 
              << ", " << result2.z() << ", " << result2.w() << ")\n\n";
    
    // Тест 3: Вращение вокруг Z на 90 градусов
    mat4x4 rot = mat4x4::rotation_z(M_PI / 2.0f);
    vec4 point3(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 result3 = rot * point3;
    
    std::cout << "Test 3 - 90° rotation around Z:\n";
    std::cout << "Expected: (~0, ~1, 0, 1)\n";
    std::cout << "Got: (" << result3.x() << ", " << result3.y() 
              << ", " << result3.z() << ", " << result3.w() << ")\n";
    
    return 0;
}
```

---

## Заключение

**Главная проблема:** Матрица translation неправильно размещает компоненты переноса. Это критическая ошибка, которая ломает всю систему трансформаций.

**Статус других методов:**
- ✅ `rotation_x/y/z` — похоже на корректные
- ✅ `scaling` — корректна
- ⚠️ `make_model_matrix` — порядок требует проверки после исправления translation
- ✅ `operator*(vec4)` — корректна для column-vector семантики

**Приоритет исправления:** ВЫСОКИЙ — без правильной translation вся библиотека трансформаций непригодна для использования.
