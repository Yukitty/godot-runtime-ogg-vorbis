extends Control


func _ready() -> void:
	$TestSound.stream = ResourceLoaderOggVorbis.load("res://harmonica.ogg")


func _on_test_button_pressed() -> void:
	$TestSound.play()
