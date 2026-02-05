package it.unibo.iot03.com;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import it.unibo.iot03.controller.Controller;
import it.unibo.iot03.model.Data;
import it.unibo.iot03.model.api.Tms;

/*
 * Data Service as a vertx event-loop
 */
public class HttpServer extends AbstractVerticle {

	private int port;
	private final Tms tms;
	private final Controller controller;

	public HttpServer(final int port, final Tms logic, final Controller controller) {
		this.port = port;
		this.tms = logic;
		this.controller = controller;
	}

	@Override
	public void start() {
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data").handler(this::handleGetData);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready on port: " + port);
	}

	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		log("New msg "+routingContext.body().asString());
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			if (res.containsKey("toggle")) {
				if (this.controller.toggleMode()) {
					response.setStatusCode(200).end();
				} else {
					sendError(400, response);
				}
				this.log("Requested state toggle");
			} else if (res.containsKey("value")) {
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

	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[HTTP SERVER] " + msg);
	}

}