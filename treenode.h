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
    void setValue(const double &newValue);

    /*!
     * \brief Устанавливает левый дочерний узел
     * \param новый узел.
     */
    void setLeftChild(TreeNode *left);

    /*!
     * \brief Устанавливает правый дочерний узел
     * \param новый узел.
     */
    void setRightChild(TreeNode *right);

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
     * \brief Возвращает левый дочерний узел.
     * \return дочерний узел.
     */
    TreeNode *getLeftChild() const;

    /*!
     * \brief Возвращает правый дочерний узел.
     * \return дочерний узел.
     */
    TreeNode *getRightChild() const;

    /*!
     * \brief Оператор присваивания.
     * \param other Другой узел, который присваивается.
     * \return Ссылка на текущий узел после присваивания.
     */
    TreeNode &operator=(const TreeNode &);

private:
    /// Значение
    double _value = 0;
    /// Родительский узел.
    TreeNode *_parent = nullptr;
    /// Левый дочерний узел
    TreeNode *_left = nullptr;
    /// Правый дочерний узел
    TreeNode *_right = nullptr;
};

#endif // TREENODE_H
