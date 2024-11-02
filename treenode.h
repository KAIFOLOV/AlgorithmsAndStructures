#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

class TreeNode
{
public:
    /*!
     * \brief Конструктор по умолчанию.
     * \param parent Родительский узел.
     */
    explicit TreeNode(TreeNode *parent = nullptr);
    /*!
     * \brief Конструктор с указанием ключа.
     * \param parent Родительский узел.
     */
    explicit TreeNode(const TreeNode &other);

    ~TreeNode();

    // Setters

    /*!
     * \brief Устанавливает родительский узел.
     * \param parent Родительский узел.
     */
    void setParent(TreeNode *parent);

    /*!
     * \brief Устанавливает значение для узла
     * \param новое значение.
     */
    void setValue(double newValue);

    // Getters

    /*!
     * \brief Возвращает значение узла
     * \return Значение
     */
    double getValue() const;

    /*!
     * \brief Возвращает родительский узел.
     * \return Родительский узел.
     */
    TreeNode *getParent() const;

    /*!
     * \brief Возвращает дочерний узел по индексу.
     * \param index Индекс дочернего узла.
     * \return Дочерний узел.
     */
    TreeNode *getChild(int index);

    /*!
     * \brief Возвращает список дочерних узлов.
     * \return Список дочерних узлов.
     */
    std::vector<TreeNode *> getChildren() const;

    // Child

    /*!
     * \brief Возвращает количество дочерних узлов.
     * \return Количество дочерних узлов.
     */
    int childCount() const;

    /*!
     * \brief Добавляет дочерний узел.
     * \param child Дочерний узел.
     */
    void addChild(TreeNode *);

    /*!
     * \brief Удаляет дочерний узел.
     * \param child Дочерний узел.
     */
    void removeChild(TreeNode *child);

    /*!
     * \brief Возвращает индекс указанного узла среди дочерних узлов.
     * \param node Узел, для которого ищется индекс.
     * \return Индекс узла.
     */
    int indexOfChild(TreeNode *node);

    /*!
     * \brief Оператор присваивания.
     * \param other Другой узел, который присваивается.
     * \return Ссылка на текущий узел после присваивания.
     */
    TreeNode &operator=(const TreeNode &);

private:
    /// Значение
    double _value = 0;
    /// Список дочерних узлов
    std::vector<TreeNode *> _children;
    /// Родительский узел.
    TreeNode *_parent = nullptr;
};

#endif // TREENODE_H
