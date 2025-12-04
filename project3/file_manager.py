import shutil
import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox, simpledialog
from pathlib import Path
from datetime import datetime

class FileManagerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("File Manager Project")
        self.root.geometry("1000x600")

        # Gets path to start program at
        self.current_path = Path.cwd()

        # Sets up the UI
        self.setup_top_bar()
        self.setup_main_view()
        self.setup_status_bar()

        # Load initial files
        self.refresh_files()

    # Creates the top address bar and navigation buttons
    def setup_top_bar(self):
        bar = ttk.Frame(self.root, padding=10)
        bar.pack(fill=tk.X)

        ttk.Label(bar, text="Path:").pack(side=tk.LEFT)

        self.path_var = tk.StringVar()
        ttk.Entry(bar, textvariable=self.path_var, state="readonly").pack(
            side=tk.LEFT, fill=tk.X, expand=True, padx=10
        )

        ttk.Button(bar, text="Up", command=self.go_up).pack(side=tk.RIGHT)
        ttk.Button(bar, text="Refresh", command=self.refresh_files).pack(side=tk.RIGHT, padx=5)

    # Creates the main file list and navigation buttons
    def setup_main_view(self):
        main = ttk.Frame(self.root, padding=10)
        main.pack(fill=tk.BOTH, expand=True)

        # Creates a treeview for name, size and file type
        columns = ("name", "size", "type")
        self.tree = ttk.Treeview(main, columns=columns, show="headings", selectmode="browse")

        self.tree.heading("name", text="Name", anchor=tk.W)
        self.tree.heading("size", text="Size", anchor=tk.W)
        self.tree.heading("type", text="Type", anchor=tk.W)

        self.tree.column("name", width=400)
        self.tree.column("size", width=100)
        self.tree.column("type", width=100)

        # Adds scrollbar
        scrollbar = ttk.Scrollbar(main, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)

        self.tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.LEFT, fill=tk.Y)

        # Opens folder/file if double clicked
        self.tree.bind("<Double-1>", self.on_double_click)

        btn_frame = ttk.LabelFrame(main, text="Actions", padding=10)
        btn_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=(10, 0))

        ttk.Button(btn_frame, text="New File", command=self.create_file, width=15).pack(pady=3)
        ttk.Button(btn_frame, text="New Folder", command=self.create_folder, width=15).pack(pady=3)
        ttk.Button(btn_frame, text="Open", command=self.open_selected, width=15).pack(pady=3)
        ttk.Button(btn_frame, text="Rename", command=self.rename_selected, width=15).pack(pady=3)
        ttk.Button(btn_frame, text="Delete", command=self.delete_selected, width=15).pack(pady=3)
        ttk.Button(btn_frame, text="Properties", command=self.show_properties, width=15).pack(pady=3)

    def setup_status_bar(self):
        self.status_var = tk.StringVar(value="Ready")
        ttk.Label(self.root, textvariable=self.status_var, relief=tk.SUNKEN, padding=5).pack(side=tk.BOTTOM, fill=tk.X)

    # Refreshes files and reloads files in the current directory
    # GPT-5.1 was used to help debug this function as I had some trouble getting it work initially
    def refresh_files(self):
        self.path_var.set(str(self.current_path))

        # Clear existing items
        for item in self.tree.get_children():
            self.tree.delete(item)

        try:
            # Sorts directories and files, with directories being sorted first
            items = sorted(self.current_path.iterdir(), key=lambda x: (not x.is_dir(), x.name.lower()))

            for item in items:
                # Get details
                if item.is_dir():
                    size = "-"
                    kind = "Folder"
                else:
                    size = self.format_size(item.stat().st_size)
                    kind = "File"

                # Insert into tree. Stores the filename in the first column.
                self.tree.insert("", tk.END, iid=str(item), values=(item.name, size, kind))

            self.status_var.set(f"Loaded {len(items)} items.")

        except Exception as e:
            messagebox.showerror("Error", f"Access denied: {e}")

    # Gets actual path of user selected item
    def get_selected_path(self):
        selection = self.tree.selection()
        if not selection:
            return None
        return Path(selection[0])

    # Goes up an directory
    def go_up(self):
        if self.current_path.parent != self.current_path:
            self.current_path = self.current_path.parent
            self.refresh_files()

    # Navigates to directory if directory double clicked or open file if file is double clicked
    def on_double_click(self, event):
        path = self.get_selected_path()
        if path and path.is_dir():
            self.current_path = path
            self.refresh_files()
        elif path:
            self.open_editor(path)

    # Opens selected directory/file if button pressed
    def open_selected(self):
        path = self.get_selected_path()
        if not path:
            return
        if path.is_dir():
            self.current_path = path
            self.refresh_files()
        else:
            self.open_editor(path)

    # Creates file with user specified filename
    def create_file(self):
        name = simpledialog.askstring("New File", "Filename:")
        if name:
            new_path = self.current_path / name
            if new_path.exists():
                messagebox.showerror("Error", "File already exists.")
                return
            new_path.touch()
            self.refresh_files()

    # Creates folder with user specified filename
    def create_folder(self):
        name = simpledialog.askstring("New Folder", "Folder Name:")
        if name:
            new_path = self.current_path / name
            if new_path.exists():
                messagebox.showerror("Error", "Folder already exists.")
                return
            new_path.mkdir()
            self.refresh_files()

    # Deletes selected file
    def delete_selected(self):
        path = self.get_selected_path()
        if path and messagebox.askyesno("Delete", f"Permanently delete {path.name}?"):
            try:
                if path.is_dir():
                    shutil.rmtree(path)
                else:
                    path.unlink()
                self.refresh_files()
            except Exception as e:
                messagebox.showerror("Error", str(e))

    # Renames selected file
    def rename_selected(self):
        path = self.get_selected_path()
        if not path:
            return

        new_name = simpledialog.askstring("Rename", "New Name:", initialvalue=path.name)
        if new_name:
            try:
                path.rename(path.parent / new_name)
                self.refresh_files()
            except Exception as e:
                messagebox.showerror("Error", str(e))

    # Shows file properties
    def show_properties(self):
        path = self.get_selected_path()
        if not path: return

        stat = path.stat()
        info = (
            f"Name: {path.name}\n"
            f"Path: {path}\n"
            f"Size: {stat.st_size} bytes\n"
            f"Modified: {datetime.fromtimestamp(stat.st_mtime)}"
        )
        messagebox.showinfo("Properties", info)

    # Opens editor to edit selected file (cannot be binary file)
    def open_editor(self, path):
        try:
            content = path.read_text(encoding='utf-8')
        except Exception:
            messagebox.showerror("Error", "Can only open text files")
            return

        editor = tk.Toplevel(self.root)
        editor.title(f"Editing - {path.name}")
        editor.geometry("600x400")

        text_area = scrolledtext.ScrolledText(editor)
        text_area.pack(fill=tk.BOTH, expand=True)
        text_area.insert(tk.END, content)

        def save():
            path.write_text(text_area.get(1.0, tk.END).strip(), encoding='utf-8')
            editor.destroy()
            self.status_var.set(f"Saved {path.name}")

        ttk.Button(editor, text="Save & Close", command=save).pack(fill=tk.X)

    # Formats size in appropriate, human-readable units
    def format_size(self, size):
        for unit in ['B', 'KB', 'MB', 'GB']:
            if size < 1024:
                return f"{size:.1f} {unit}"
            size /= 1024
        return f"{size:.1f} TB"


if __name__ == "__main__":
    root = tk.Tk()
    app = FileManagerApp(root)
    root.mainloop()
