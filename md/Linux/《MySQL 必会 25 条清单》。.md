# 《MySQL 必会 25 条清单》**。
这 25 条覆盖 **80%–90% 实际开发场景**，尤其适合你后续做 **C/C++ 后端、网络服务、简单 Web 系统**。

# 一、数据库与表（6 条）

### 1️⃣ 查看所有数据库

```sql
SHOW DATABASES;
```

### 2️⃣ 使用数据库

```sql
USE db_name;
```

### 3️⃣ 查看当前数据库的表

```sql
SHOW TABLES;
```

### 4️⃣ 创建表

```sql
CREATE TABLE student
(
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(32) NOT NULL,
    age INT,
    gender VARCHAR(2)
);
```

### 5️⃣ 查看表结构

```sql
DESC student;
```

### 6️⃣ 删除表

```sql
DROP TABLE student;
```

------

# 二、数据增删改查（CRUD，8 条）

### 7️⃣ 插入数据

```sql
INSERT INTO student (name, age, gender)
VALUES ('张三', 20, '男');
```

### 8️⃣ 查询所有数据

```sql
SELECT * FROM student;
```

### 9️⃣ 查询指定列

```sql
SELECT name, age FROM student;
```

### 🔟 条件查询

```sql
SELECT * FROM student WHERE age > 18;
```

### 1️⃣1️⃣ 多条件查询

```sql
SELECT * FROM student
WHERE age > 18 AND gender = '男';
```

### 1️⃣2️⃣ 修改数据

```sql
UPDATE student
SET age = 21
WHERE name = '张三';
```

### 1️⃣3️⃣ 删除数据

```sql
DELETE FROM student WHERE name = '张三';
```

### 1️⃣4️⃣ 排序

```sql
SELECT * FROM student ORDER BY age DESC;
```

------

# 三、查询增强（6 条）

### 1️⃣5️⃣ 限制条数（分页核心）

```sql
SELECT * FROM student LIMIT 5;
```

### 1️⃣6️⃣ 分页查询

```sql
SELECT * FROM student LIMIT 5 OFFSET 10;
```

### 1️⃣7️⃣ 模糊查询

```sql
SELECT * FROM student WHERE name LIKE '%张%';
```

### 1️⃣8️⃣ 去重

```sql
SELECT DISTINCT gender FROM student;
```

### 1️⃣9️⃣ 范围查询

```sql
SELECT * FROM student WHERE age BETWEEN 18 AND 25;
```

### 2️⃣0️⃣ 集合查询

```sql
SELECT * FROM student WHERE age IN (18, 20, 22);
```

------

# 四、统计与分组（5 条）

### 2️⃣1️⃣ 统计行数

```sql
SELECT COUNT(*) FROM student;
```

### 2️⃣2️⃣ 最大 / 最小值

```sql
SELECT MAX(age), MIN(age) FROM student;
```

### 2️⃣3️⃣ 平均值

```sql
SELECT AVG(age) FROM student;
```

### 2️⃣4️⃣ 分组统计

```sql
SELECT gender, COUNT(*)
FROM student
GROUP BY gender;
```

### 2️⃣5️⃣ 分组后筛选

```sql
SELECT gender, COUNT(*)
FROM student
GROUP BY gender
HAVING COUNT(*) > 1;
```

------

# 五、这 25 条你学到什么程度算“熟练”？

✅ 看到需求，**不用想就知道用哪几条**
✅ 能组合写出 3～5 行的查询
✅ 知道什么时候用 `WHERE`，什么时候用 `HAVING`
✅ 明白 `LIMIT` 就是分页

如果你能**不看资料写出其中 15～20 条**，已经是**合格后端水平**。

------

# 六、给你的实战建议（非常关键）

你可以用这 25 条完成：

- 登录校验（`SELECT ... WHERE`）
- 注册（`INSERT` + `UNIQUE`）
- 列表分页（`SELECT + LIMIT`）
- 排行榜（`ORDER BY`）
- 数据统计（`GROUP BY`）

**不需要再多学 SQL 本身了**，下一步应该是：

> **SQL + 程序语言一起用**

------

## 下一步我可以直接带你做三选一：

1️⃣ **用 C++ 调 MySQL 写登录系统**
2️⃣ **把 student 表映射成 C++ struct**
3️⃣ **给你 10 道 MySQL 实战题（含答案）**

你直接选一个即可。