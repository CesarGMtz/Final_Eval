import sys
import os
import time
import subprocess
import re

from PyQt5.QtWidgets import (
   QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel, QPushButton, QFileDialog, QProgressBar, QTextEdit, QAction, QSizePolicy,
   QSlider, QHBoxLayout
)
from PyQt5.QtCore import QThread, pyqtSignal, Qt
from PyQt5.QtGui import QFontMetrics


class ProcessorThread(QThread):
    processing_finished = pyqtSignal(int)
    error_occurred = pyqtSignal(str)

    def __init__(self, command, project_root, arc_file_path):
        super().__init__()
        self.command = command
        self.project_root = project_root
        self.arc_file_path = arc_file_path

    def run(self):
        try:
            result = subprocess.run(self.command, capture_output=True, text=True, cwd=os.path.join(self.project_root, "src"))
            if result.returncode != 0:
                error_message = f"Error in processing:\n"
                if result.stderr:
                    error_message += result.stderr
                error_message += f"Process finished with error code: {result.returncode}"
                self.error_occurred.emit(error_message)
            self.processing_finished.emit(result.returncode)
        except Exception as e:
            self.error_occurred.emit(f"Failed to execute process: {e}")


class ProgressMonitorThread(QThread):
    progress = pyqtSignal(int)
    info_update = pyqtSignal(str)
    finished_monitoring = pyqtSignal()

    def __init__(self, output_folder, total_expected_images):
        super().__init__()
        self.output_folder = output_folder
        self.total_expected_images = total_expected_images * 6
        self._is_running = True

    def run(self):
        last_count = 0
        while self._is_running:
            try:
                current_count = 0
                if os.path.exists(self.output_folder):
                    current_count = len([name for name in os.listdir(self.output_folder) if os.path.isfile(os.path.join(self.output_folder, name))])
                if current_count > last_count:
                    last_count = current_count
                    if self.total_expected_images > 0:
                        progress_percent = int((current_count / self.total_expected_images) * 100)
                        self.progress.emit(progress_percent)
                        self.info_update.emit(f"Imagenes generadas: {current_count}/{self.total_expected_images}")
                if self.total_expected_images > 0 and current_count >= self.total_expected_images:
                    self._is_running = False
            except Exception as e:
                self.info_update.emit(f"Error en el monitoreo de: {e}")
            time.sleep(1)
        self.finished_monitoring.emit()

    def stop(self):
        self._is_running = False


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Procesador de Archivos")
        self.input_folder = ""
        self.output_folder = ""
        self.total_images_to_process = 0
        self.processor_thread = None
        self.monitor_thread = None

        main_widget = QWidget()
        layout = QVBoxLayout()

        self.input_label = QLabel("Carpeta de entrada: No seleccionada")
        self.input_label.setSizePolicy(QSizePolicy.Ignored, QSizePolicy.Preferred)
        self.input_label.setMinimumWidth(400)
        self.input_label.setWordWrap(False)
        self.input_label.setTextInteractionFlags(Qt.TextSelectableByMouse)

        self.output_label = QLabel("Carpeta de salida: No determinada")
        self.select_input_button = QPushButton("Seleccionar carpeta de entrada")
        self.select_input_button.clicked.connect(self.select_input_folder)

        self.kernel_label = QLabel("Kernel: 55")
        self.kernel_slider = QSlider(Qt.Horizontal)
        self.kernel_slider.setMinimum(55)
        self.kernel_slider.setMaximum(155)
        self.kernel_slider.setValue(55)
        self.kernel_slider.setTickInterval(10)
        self.kernel_slider.setTickPosition(QSlider.TicksBelow)
        self.kernel_slider.valueChanged.connect(self.update_kernel_label)

        kernel_layout = QHBoxLayout()
        kernel_layout.addWidget(self.kernel_label)
        kernel_layout.addWidget(self.kernel_slider)

        self.progress_bar = QProgressBar()
        self.progress_bar.setValue(0)

        self.info_display = QTextEdit()
        self.info_display.setReadOnly(True)

        self.process_button = QPushButton("Iniciar procesamiento")
        self.process_button.clicked.connect(self.start_processing)

        self.reset_button = QPushButton("Reiniciar Sistema")
        self.reset_button.clicked.connect(self.reset_system)

        layout.addWidget(self.input_label)
        layout.addWidget(self.output_label)
        layout.addWidget(self.select_input_button)
        layout.addLayout(kernel_layout)
        layout.addWidget(self.process_button)
        layout.addWidget(self.reset_button)
        layout.addWidget(QLabel("Progreso estimado:"))
        layout.addWidget(self.progress_bar)
        layout.addWidget(QLabel("Información del procesamiento:"))
        layout.addWidget(self.info_display)

        main_widget.setLayout(layout)
        self.setCentralWidget(main_widget)

        menubar = self.menuBar()
        team_menu = menubar.addMenu("Equipo")
        team_members = [
            "César Guerra Martínez | A01656774",
            "Gerardo Dehustúa Hernández | A01736455",
            "José Luis Zago Guevara | A01736278"
        ]
        for name in team_members:
            member_action = QAction(name, self)
            team_menu.addAction(member_action)

    def update_kernel_label(self, value):
        self.kernel_label.setText(f"Kernel: {value}")

    def set_elided_text(self, label, text):
        metrics = QFontMetrics(label.font())
        elided = metrics.elidedText(text, Qt.ElideMiddle, label.width())
        label.setText(elided)
        label.setToolTip(text)

    def select_input_folder(self):
        folder = QFileDialog.getExistingDirectory(self, "Seleccionar carpeta de entrada")
        if folder:
            self.input_folder = folder
            self.set_elided_text(self.input_label, f"Carpeta de entrada: {folder}")
            self.count_images_in_input_folder()

    def count_images_in_input_folder(self):
        self.total_images_to_process = 0
        if os.path.isdir(self.input_folder):
            for filename in os.listdir(self.input_folder):
                if re.match(r"\d+\.bmp", filename):
                    self.total_images_to_process += 1
        self.info_display.append(f"Se encontraron {self.total_images_to_process} imágenes BMP en la carpeta de entrada.")

    def generate_machinefile(self):
        current_script_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(current_script_dir)
        script_path = os.path.join(project_root, "src", "check_hosts.sh")
        result = subprocess.run(["bash", script_path], capture_output=True, text=True)
        self.info_display.append(result.stdout)
        if result.stderr:
            self.info_display.append("Errores durante la generación del machinefile:")
            self.info_display.append(result.stderr)
        machinefile_path = os.path.join(project_root, "src", "machinefile")
        return os.path.exists(machinefile_path) and os.path.getsize(machinefile_path) > 0

    def start_processing(self):
        if not self.input_folder:
            self.info_display.append("Selecciona una carpeta de entrada primero.")
            return

        if self.total_images_to_process == 0:
            self.info_display.append("No se encontraron imágenes para procesar en la carpeta de entrada.")
            return

        current_script_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(current_script_dir)
        output_image_dir = os.path.join(project_root, "r_img")
        arc_file_path = os.path.join(project_root, "src", "arc1.txt")

        os.makedirs(output_image_dir, exist_ok=True)
        self.output_folder = output_image_dir
        self.set_elided_text(self.output_label, f"Carpeta de salida: {self.output_folder}")

        kernel = self.kernel_slider.value()
        exe_path = os.path.join(project_root, "src", "procesador") 

        if not os.path.exists(exe_path):
            self.info_display.append(f"No se encontró el ejecutable del procesador en: {exe_path}")
            return

        try:
            with open(arc_file_path, 'w') as f:
                f.write("")
        except Exception as e:
            self.info_display.append(f"Error limpiando arc1.txt: {e}")

        for file_name in os.listdir(output_image_dir):
            file_path = os.path.join(output_image_dir, file_name)
            try:
                if os.path.isfile(file_path):
                    os.unlink(file_path)
            except Exception as e:
                self.info_display.append(f"Error limpiando imágenes del folder: {e}")

        # Ejecutar script bash para generar machinefile y obtener NUM_SLOTS
        machinefile_script = os.path.join(project_root, "src", "check_hosts.sh")  # Ajusta la ruta si es necesario

        try:
            output = subprocess.check_output(["bash", machinefile_script], text=True)
            num_procs = int(output.strip())
            self.info_display.append(f"Número de slots activos detectados: {num_procs}")
        except Exception as e:
            self.info_display.append(f"Error al obtener número de slots: {e}")
            num_procs = 1  # fallback si falla la detección

        machinefile_path = os.path.join(project_root, "src", "machinefile")

        command = [
            "mpiexec",
            "-n", str(num_procs),
            "--hostfile", machinefile_path,
            exe_path,
            self.input_folder,
            output_image_dir,
            arc_file_path,
            str(kernel),
            str(self.total_images_to_process)
        ]

        self.info_display.append("Iniciando procesamiento...")
        self.progress_bar.setValue(0)
        self.select_input_button.setEnabled(False)
        self.process_button.setEnabled(False)
        self.reset_button.setEnabled(False)

        self.processor_thread = ProcessorThread(command, project_root, arc_file_path)
        self.processor_thread.processing_finished.connect(self.on_processing_finished)
        self.processor_thread.error_occurred.connect(self.on_processing_error)
        self.processor_thread.start()

        self.monitor_thread = ProgressMonitorThread(output_image_dir, self.total_images_to_process)
        self.monitor_thread.progress.connect(self.progress_bar.setValue)
        self.monitor_thread.info_update.connect(self.info_display.append)
        self.monitor_thread.finished_monitoring.connect(self.on_monitoring_finished)
        self.monitor_thread.start()


    def on_processing_finished(self, return_code):
        self.info_display.append("Procesamiento completado por el ejecutable.")
        if self.monitor_thread and not self.monitor_thread.isRunning():
            self.progress_bar.setValue(100)

        current_script_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(current_script_dir)
        arc_file_path = os.path.join(project_root, "src", "arc1.txt")

        try:
            with open(arc_file_path, 'r') as f:
                arc_content = f.readlines()

            results = {"leidas": "N/A", "escritas": "N/A", "tasa": "N/A"}
            for line in arc_content:
                if "Localidades leidas:" in line:
                    results["leidas"] = line.split(":")[1].strip()
                elif "Localidades escritas:" in line:
                    results["escritas"] = line.split(":")[1].strip()
                elif "Tasa MB/s:" in line:
                    results["tasa"] = line.split(":")[1].strip()

            self.info_display.append("\nResultados finales:")
            self.info_display.append(f"  Total de localidades leidas: {results['leidas']}")
            self.info_display.append(f"  Total de localidades escritas: {results['escritas']}")
            self.info_display.append(f"  Tasa total de MB procesados (MB/segundo): {results['tasa']}")
        except Exception as e:
            self.info_display.append(f"Error leyendo arc1.txt: {e}")
        finally:
            self.select_input_button.setEnabled(True)
            self.process_button.setEnabled(True)
            self.reset_button.setEnabled(True)
            if self.monitor_thread and self.monitor_thread.isRunning():
                self.monitor_thread.stop()
                self.monitor_thread.wait()

    def on_processing_error(self, message):
        self.info_display.append(message)
        self.select_input_button.setEnabled(True)
        self.process_button.setEnabled(True)
        self.reset_button.setEnabled(True)
        if self.monitor_thread and self.monitor_thread.isRunning():
            self.monitor_thread.stop()
            self.monitor_thread.wait()

    def on_monitoring_finished(self):
        self.info_display.append("\nMonitoreo de progreso finalizado.")
        self.progress_bar.setValue(100)

    def reset_system(self):
        if self.processor_thread and self.processor_thread.isRunning():
            self.processor_thread.quit()
            self.processor_thread.wait()
            self.info_display.append("Procesamiento detenido.")

        if self.monitor_thread and self.monitor_thread.isRunning():
            self.monitor_thread.stop()
            self.monitor_thread.quit()
            self.monitor_thread.wait()
            self.info_display.append("Monitoreo detenido.")

        self.input_folder = ""
        self.output_folder = ""
        self.total_images_to_process = 0
        self.set_elided_text(self.input_label, "Carpeta de entrada: No seleccionada")
        self.output_label.setText("Carpeta de salida: No determinada")
        self.progress_bar.setValue(0)
        self.info_display.clear()
        self.select_input_button.setEnabled(True)
        self.process_button.setEnabled(True)
        self.reset_button.setEnabled(True)
        self.kernel_slider.setValue(55)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
