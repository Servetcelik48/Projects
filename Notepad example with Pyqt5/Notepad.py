"""
Date: 11.08.2021
Created by Servet Çelik

"""

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtPrintSupport import *
import os,sys

class mainwiindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.editor = QPlainTextEdit()
        self.setCentralWidget(self.editor)

        font = QFontDatabase.systemFont(QFontDatabase.FixedFont)
        font.setPointSize(12)
        self.editor.setFont(font)

        self.path = None

        status = self.statusBar()
        self.setStatusBar(status)

        toolbar = QToolBar()
        toolbar.setIconSize(QSize(14,14))
        self.addToolBar(toolbar)

        menubar = QMenuBar()
        menubar = self.menuBar()

        dosya_menu = menubar.addMenu("File")
        ekle_menu = menubar.addMenu("Edit")

        dosya_ac = QAction(QIcon(os.path.join("open_file.png")), "Open file",self)
        dosya_ac.setStatusTip("allows to open an external file")
        dosya_ac.setShortcut("Ctrl+O")
        toolbar.addAction(dosya_ac)
        dosya_menu.addAction(dosya_ac)

        kaydet = QAction(QIcon(os.path.join("save.png")), "Save", self)
        kaydet.setStatusTip("Saves the file")
        kaydet.setShortcut("Ctrl+S")
        toolbar.addAction(kaydet)
        dosya_menu.addAction(kaydet)

        farkli_kaydet = QAction(QIcon(os.path.join("save_as.png")),"Save as",self)
        farkli_kaydet.setStatusTip("allows to edit saving location and type ")
        toolbar.addAction(farkli_kaydet)
        dosya_menu.addAction(farkli_kaydet)

        yazdir = QAction(QIcon(os.path.join("print.png")),"Print",self)
        yazdir.setStatusTip("Print the file")
        yazdir.setShortcut("Ctrl+P")
        toolbar.addAction(yazdir)
        dosya_menu.addAction(yazdir)

        geri_al = QAction(QIcon(os.path.join("undo.png")), "Undo",self)
        geri_al.setShortcut("Ctrl+Z")
        geri_al.setStatusTip("One step back")
        toolbar.addAction(geri_al)
        ekle_menu.addAction(geri_al)

        ileri_al = QAction(QIcon(os.path.join("redo.png")),"Redo",self)
        ileri_al.setShortcut("CTrl+Y")
        ileri_al.setStatusTip("One step forward")
        toolbar.addAction(ileri_al)
        ekle_menu.addAction(ileri_al)

        kes = QAction(QIcon(os.path.join("cut.png")), "Cut", self)
        kes.setShortcut("CTrl+X")
        kes.setStatusTip("Cut selected text")
        toolbar.addAction(kes)
        ekle_menu.addAction(kes)

        kopyala = QAction(QIcon(os.path.join("copy.png")), "Copy", self)
        kopyala.setShortcut("CTrl+C")
        kopyala.setStatusTip("Copy selected text")
        toolbar.addAction(kopyala)
        ekle_menu.addAction(kopyala)

        yapistir = QAction(QIcon(os.path.join("paste.png")), "Paste", self)
        yapistir.setShortcut("CTrl+V")
        yapistir.setStatusTip("Paste the copied items")
        toolbar.addAction(yapistir)
        ekle_menu.addAction(yapistir)

        hepsini_seç = QAction(QIcon(os.path.join("select_all.png")), "Select all", self)
        hepsini_seç.setShortcut("CTrl+A")
        hepsini_seç.setStatusTip("Selects all items")
        toolbar.addAction(hepsini_seç)
        ekle_menu.addAction(hepsini_seç)


        dosya_ac.triggered.connect(self.dosya_ac_def)
        kaydet.triggered.connect(self.kaydet_def)
        farkli_kaydet.triggered.connect(self.farkli_kaydet_def)
        yazdir.triggered.connect(self.yazdir_def)
        geri_al.triggered.connect(self.editor.undo)
        ileri_al.triggered.connect(self.editor.redo)
        kes.triggered.connect(self.editor.cut)
        kopyala.triggered.connect(self.editor.copy)
        yapistir.triggered.connect(self.editor.paste)
        hepsini_seç.triggered.connect(self.editor.selectAll)







        self.basligi_düzenle()
        self.setGeometry(100,100,500,500)
        self.show()
    def basligi_düzenle(self):
        self.setWindowTitle("{} - NotePad".format(os.path.basename(self.path) if self.path else "untitled"))

    def hata_mesaj(self,mesaj):
        hata = QMessageBox()
        hata.setText(mesaj)
        hata.setIcon(QMessageBox.critical)
        hata.show()

    def dosya_ac_def(self):
        path , _ = QFileDialog.getOpenFileName(self,"Open File","","Text Files(*.txt)")
        if not path:
            return
        if path:
            try:
                with open(path,"r") as file:
                    text = file.read()
            except Exception as e:
                self.hata_mesaj(e)
            else:
                self.editor.setPlainText(text)
                self.path = path
                self.basligi_düzenle()

    def kaydet_def(self):
        if self.path == None:
            return self.farkli_kaydet_def()
        text = self.editor.toPlainText()
        try:
            with open(self.path,"w") as file:
                file.write(text)
        except Exception as e:
            self.hata_mesaj(e)
        else:
            pass


    def farkli_kaydet_def(self):
        path,_ = QFileDialog.getSaveFileName(self,"Save as","", "Text Files(*.txt)")
        if not path:
            return
        text = self.editor.toPlainText()
        try:
            with open(path,"w") as file:
                file.write(text)
        except Exception as e:
            self.hata_mesaj(e)
        else:
            self.path = path
            self.basligi_düzenle()



    def yazdir_def(self):
        mesaj = QPrintDialog()
        if mesaj.exec_():
            self.editor.print_(mesaj.printer())


if (__name__ == "__main__"):
    app = QApplication(sys.argv)
    app.setApplicationName("NotePad")
    window1 = mainwiindow()
    app.exec_()
