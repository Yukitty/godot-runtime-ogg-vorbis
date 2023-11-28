extends Control


func _on_test_button_pressed() -> void:
	$TestSound.stream = ResourceLoader.load("res://harmonica.ogg")
	$TestSound.play()
