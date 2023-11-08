/*
 * class: CFillPictureWidget
 * biref: 生成填充图片的配置对话框
 * 平均功能
        |——对话框CFillPictureDialog 配置结束，根据QDialog的返回结果设定画刷的填充图片位置和平铺镜像
 */
#ifndef FILLPICTUREDIALOG_H
#define FILLPICTUREDIALOG_H

#include <QDialog>

namespace Ui {
class CFillPictureDialog;
}

class CFillPictureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFillPictureDialog(QWidget *parent = 0);
    ~CFillPictureDialog();

private:
    Ui::CFillPictureDialog *ui;

public:
    QString m_pathName;
    QString m_imageName;
    QString m_projectPath;
    int m_position;

private:
    QImage m_image;
public:
    void inintDialog();
    void setCSDisable();

private:
    void tileSetting(QPixmap pix,bool fillBack); //! 设定平铺镜像
//    void createFile(const QString &str);//!创建文件夹
    bool copyFileToPath(QString sourceDir ,QString toDir); //! 拷贝图片资源文件

protected:
    void paintEvent(QPaintEvent*);

private slots:
    void resetPath(const QString &str);
    void on_choseBtn_clicked();
    void on_centerRadioButton_clicked();
    void on_tileRadioButton_clicked();
    void on_stretchRadioButton_clicked();

public:
    //! 获取图片名
    void setImageName(const QString &imageName);
    QString imageName();
    //! 获取图片保存路径
    void setProjectPath(const QString &proPath);
    QString projectPath() const;
};

#endif // FILLPICTUREWIDGET_H
