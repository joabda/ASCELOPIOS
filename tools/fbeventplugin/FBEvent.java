package fbeventplugin;

import java.time.ZonedDateTime;
import java.time.ZonedDateTime;
import java.util.Date;

public class FBEvent {
	private String title;
	private String description;
	private String place;
	private ZonedDateTime start;
	private ZonedDateTime end;
	
	@Override
	public String toString() {
		return "FBEvent [title=" + title + ", description=" + description + ", place=" + place + ", start=" + start.toString()
				+ ", end=" + end.toString() + "]";
	}
	
	public FBEvent(String title, String description, String place, ZonedDateTime start, ZonedDateTime end) {
		super();
		this.title = title;
		this.description = description;
		this.place = place;
		this.start = start;
		this.end = end;
	}

	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public String getPlace() {
		return place;
	}
	public void setPlace(String place) {
		this.place = place;
	}
	public ZonedDateTime getStart() {
		return start;
	}
	public void setStart(ZonedDateTime start) {
		this.start = start;
	}
	public ZonedDateTime getEnd() {
		return end;
	}
	public void setEnd(ZonedDateTime end) {
		this.end = end;
	}
}
