extends Node2D

var player_position := Vector2(480, 270)
var player_speed := 260.0

func _process(delta: float) -> void:
	var direction := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	player_position += direction * player_speed * delta
	player_position.x = clampf(player_position.x, 16.0, 944.0)
	player_position.y = clampf(player_position.y, 16.0, 524.0)
	queue_redraw()

func _draw() -> void:
	draw_rect(Rect2(player_position - Vector2(12, 12), Vector2(24, 24)), Color("#e16b4c"))
