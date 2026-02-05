package it.unibo.iot03.com;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;
import it.unibo.iot03.controller.Controller;
import it.unibo.iot03.model.Data;
import it.unibo.iot03.model.api.Tms;
import it.unibo.iot03.model.api.Wcs;

/*
 * Data Service as a vertx event-loop
 */
public class HttpServer extends AbstractVerticle {

	private int port;
	private final Tms tms;
	private final Wcs wcs;
	private final Controller controller;

	public HttpServer(final int port, final Tms logic, final Wcs wcs, final Controller controller) {
		this.port = port;
		this.tms = logic;
		this.wcs = wcs;
		this.controller = controller;
	}

	@Override
	public void start() {
		Router router = Router.router(vertx);
		router.route()
			.handler(
				CorsHandler.create()
					.addOrigin("*")
					.allowedMethod(HttpMethod.GET)
					.allowedMethod(HttpMethod.POST)
					.allowedHeader("Content-Type")
				);
		router.route().handler(BodyHandler.create());
		router.post("/api/opening").handler(this::handleSetOpening);
		router.get("/api/data").handler(this::handleGetData);
		router.get("/api/status").handler(this::handleGetStatus);
		router.post("/api/toggle").handler(this::handleToggle);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);
		log("Service ready on port: " + port);
	}

	// Set the opening of the valve
	private void handleSetOpening(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		log("New msg "+routingContext.body().asString());
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			if (res.containsKey("value")) {
				int value = Integer.parseInt(res.getString("value"));
				if (this.controller.setValveOpening(value)) {
					response.setStatusCode(200).end();
				} else {
					sendError(400, response);
				}
			} else {
				sendError(400, response);
			}
		}
	}

	// Send the values read by the TMS
	private void handleGetData(RoutingContext routingContext) {
		JsonArray arr = new JsonArray();
		for (Data p: this.tms.getHistory()) {
			JsonObject data = new JsonObject();
			data.put("timestamp", p.formattedTime());
			data.put("value", p.value());
			arr.add(data);
		}
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end(arr.encodePrettily());
	}

	// Switch the WCS between AUTO and MANUAL
	private void handleToggle(RoutingContext routingContext) {
		log("Toggle requested");
		this.controller.toggleMode();
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end();
	}

	// Send the current status and the valve opening value
	private void handleGetStatus(RoutingContext routingContext) {
		JsonObject data = new JsonObject();
		data.put("status", this.controller.getState());
		data.put("opening", this.wcs.getValveLevel());
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end(data.encodePrettily());
	}

	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[HTTP SERVER] " + msg);
	}

}